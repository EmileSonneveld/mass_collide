#include <string>
#include <iostream>
#include <fstream>

#if defined __APPLE__ || defined __gnu_linux__ || defined linux
#	include <unistd.h>
#elif defined _WIN32 || defined _WIN64
#	include <direct.h>
//const auto& chdir = _chdir;  // Windows wil make an alias like this, but will also throw a warning :/
#endif

// For DirExists()
#include <sys/types.h>
#include <sys/stat.h>

#include <GL/glew.h>
#include <glm/glm.hpp>
using namespace glm;
#include <glfw3.h>
#include <common/controls.hpp>
#include "globals.h"
#include "INIReader.h"

#if defined(_WIN32) || defined(WIN32)
// needed for OutputDebugStringA()
#   define WIN32_LEAN_AND_MEAN
#   include <Windows.h>
#else
// don't print to IDE on linux
void OutputDebugStringA(const char *file){}
#endif


int printOglError(const char *file, const int line)
{
	GLenum glErr;
	int    retCode = 0;

	glErr = glGetError();
	if (glErr != GL_NO_ERROR)
	{
		//std::cout << file << ":" << std::to_string(line) << " " << (char*)gluErrorString(glErr) << "\n";

		OutputDebugStringA(file);
		OutputDebugStringA(": ");
		OutputDebugStringA(std::to_string(line).c_str());
		OutputDebugStringA(": ");
		OutputDebugStringA((char*)gluErrorString(glErr));
		OutputDebugStringA("\n");
		retCode = 1;
	}
	return retCode;
}

void printTransformFeedbackValues(int nr_catch_particles)
{
	printf("--- buffer feedback ---\n");
	// get the data back to CPU
	vec4* feedback = new vec4[nr_catch_particles];
	glGetBufferSubData(GL_TRANSFORM_FEEDBACK_BUFFER, 0, nr_catch_particles * sizeof(vec4), feedback);
	for (int i = 0; i < nr_catch_particles; ++i){
		printf("feedback: %f %f %f %f\n", feedback[i].x, feedback[i].y, feedback[i].z, feedback[i].a);
	}
	delete feedback;
}

double g_cursor_x, g_cursor_y; // get set by particle smasher

extern GLFWwindow* window;

glm::vec4 CursorToWorldspace(float depth)
{
	glm::mat4 ProjectionMatrix = getProjectionMatrix();
	glm::mat4 ViewMatrix = getViewMatrix();
	glm::mat4 ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;

	auto inversed = ViewProjectionMatrix._inverse();

	int width, height;
	glfwGetWindowSize(window, &width, &height);

	glm::vec4 vector((g_cursor_x / width) * 2 - 1, (-g_cursor_y / height) * 2 + 1, 0.1, 0);
	return vector * inversed;
}

bool FileExists(const char* filename )
{
    std::ifstream openFile;
    openFile.open(filename);
    if( openFile ){
        return true;
    }
    return false;
    // openFile is on the stack and will be deleted here.
}

bool DirExists(const char* pathname){
    struct stat info;

    if( stat( pathname, &info ) != 0 ){
        printf( "cannot access %s\n", pathname );
        return false;
    }
    else if( info.st_mode & S_IFDIR )  // S_ISDIR() doesn't exist on my windows
        return true;
    else
        return false;
}

void ChangeDir(const char* dir)
{
    chdir(dir);
}

void LogCurrentDir()
{
    char * dir = getcwd(NULL, 0);
    printf("Current dir: %s\n", dir);
	delete dir;
}

INIReader* g_settings;

std::string GetPsSetting_String(const char* var_name, std::string default_value)
{
	return g_settings->Get("ps_system", var_name, default_value);
}
int GetPsSetting_Int(const char* var_name, int default_value)
{
	return g_settings->GetInteger("ps_system", var_name, default_value);
}
bool GetPsSetting_Bool(const char* var_name, bool default_value)
{
	return g_settings->GetBoolean("ps_system", var_name, default_value);
}
float GetPsSetting_Float(const char* var_name, float default_value)
{
	return g_settings->GetFloat("ps_system", var_name, default_value);
}

std::string ReplaceAll(std::string str, const std::string& from, const std::string& to)
{
	size_t start_pos = 0;
	while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
		str.replace(start_pos, from.length(), to);
		start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
	}
	return str;
}