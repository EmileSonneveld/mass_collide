#include "globals.h"
#include <string>
#include <iostream>

#include <GL/glew.h>
#include <glm/glm.hpp>
using namespace glm;
#include <glfw3.h>
#include <common/controls.hpp>

#if defined(_WIN32) || defined(WIN32)

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

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
		std::cout << file << ":" << std::to_string(line) << " " << (char*)gluErrorString(glErr) << "\n";

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
