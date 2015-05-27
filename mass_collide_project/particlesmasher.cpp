
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <string>
#include <fstream>
#include <iostream>

#include <GL/glew.h>

#include <glfw3.h>
GLFWwindow* window;

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/norm.hpp>
using namespace glm;

#include <common/shader.hpp>
#include <common/texture.hpp>
#include <common/controls.hpp>

#include "connections.h"
#include "globals.h"
#include "particle_data.h"
#include "particle_system.h"
#include "transform_feedback.h"
#include "INIReader.h"


#ifdef WIN32
#	include <signal.h>
#	define _CRT_SECURE_NO_WARNINGS
#endif

void glfw_error_callback(int error, const char* description)
{
    std::cout << "glfw error: " << description << std::endl;
}


int main(void)
{
#ifdef WIN32
	// _crtBreakAlloc= 161;
	_CrtSetBreakAlloc(1);
#endif
    
    glfwSetErrorCallback(glfw_error_callback);

	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return -1;
	}
    
    
    if(DirExists("../../mass_collide/mass_collide_project/rc")){
        ChangeDir("../../mass_collide/mass_collide_project");
        std::cout << "found recource folder and changed directory\n";
	}
	if (DirExists("../mass_collide/mass_collide_project/rc")){
		ChangeDir("../mass_collide/mass_collide_project");
		std::cout << "found recource folder and changed directory\n";
	}
    LogCurrentDir();
    
    INIReader reader("rc/settings.ini");
    
    if (reader.ParseError() < 0)
        std::cout << "Can't load .ini file\n";

    int iniOglMajor = reader.GetInteger("window", "version_major", 2);
    int iniOglMinor = reader.GetInteger("window", "version_minor", 1);
    std::cout << "Ogl version from ini: " << iniOglMajor << "." << iniOglMinor << '\n';

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, iniOglMajor); // was 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, iniOglMinor);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    auto iniWindowWidth = reader.GetInteger("window", "width", 1024);
    auto iniWindowHeight = reader.GetInteger("window", "height", 768);
    
	// Open a window and create its OpenGL context
	window = glfwCreateWindow(
                              iniWindowWidth,
                              iniWindowHeight,
                              "Tutorial 18 - Particules", NULL, NULL);
	if (window == NULL){
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	glfwSetCursorPos(window, iniWindowWidth / 2, iniWindowHeight / 2);
	glfwSwapInterval(1);
	printOpenGLError();

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}
	printOpenGLError(); // throws "invalid enumerant", just ignore it

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    
    
    
    auto iniCount = reader.GetInteger("ps_system", "count", 100);

    

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);


	// Un-used, but maby nedded for some pipeline stuff
	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);


	glfwSetCursorPos(window, 1024 / 2, 768 / 2);

	//////////////////////////////////////////////////////////////
	particle_system particle_system_inst;
	particle_system_inst.initialize();

	particle_data particle_data_inst;
    particle_data_inst.COUNT = iniCount;
	initialize_buffers(particle_data_inst);
	initialize_velocity(particle_data_inst);

	transform_feedback transform_positions;
	transform_positions.initialize("rc/compute.glsl");

	transform_feedback transform_velocities;
	transform_velocities.initialize("rc/forces.glsl");

	connections connections_inst;
	connections_inst.initialize();
	connections_inst.initialize_buffers(particle_data_inst);
	//////////////////////////////////////////////////////////////
	bool isFirstTime = true;

	do
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		printOpenGLError();
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		if (!isFirstTime)
			computeMatricesFromInputs();
		isFirstTime = false;

		glfwGetCursorPos(window, &g_cursor_x, &g_cursor_y);

		//////////////////////////////////////////////////////////////
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
			printOpenGLError();
			transform_positions.ProccesPositions(particle_data_inst);
			printOpenGLError();
			transform_velocities.ProccesVelocities(particle_data_inst);
			printOpenGLError();
		}
		if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS){
			transform_positions.initialize("rc/compute.glsl");
			transform_velocities.initialize("rc/forces.glsl");
		}
		if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS){
			initialize_buffers(particle_data_inst);
		}
		particle_system_inst.draw(particle_data_inst);
		if (glfwGetKey(window, GLFW_KEY_C) != GLFW_PRESS){
			connections_inst.draw(particle_data_inst);
			printOpenGLError();
		}
		//////////////////////////////////////////////////////////////


		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
	glfwWindowShouldClose(window) == 0);


	printOpenGLError();

	//////////////////////////////////////////////////////////////
	particle_data_inst.clean();
	particle_system_inst.clean();
	transform_positions.clean();
	//////////////////////////////////////////////////////////////


	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	printOpenGLError();

#ifdef WIN32
	_CrtDumpMemoryLeaks();
#endif

	return 0;
}
