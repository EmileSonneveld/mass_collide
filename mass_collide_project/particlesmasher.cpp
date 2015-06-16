
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

#include <AntTweakBar.h>

#include <common/shader.hpp>
#include <common/texture.hpp>
#include <common/controls.hpp>

#include "globals.h"
#include "particle_data.h"
#include "c_connections_draw.h"
#include "c_connections_transform_feedback.h"
#include "c_particle_draw.h"
#include "c_transform_feedback.h"
#include "INIReader.h"


#ifdef WIN32
#	include <signal.h>
#	define _CRT_SECURE_NO_WARNINGS
#endif

void glfw_error_callback(int error, const char* description)
{
	std::cout << "glfw error: " << description << std::endl;
}

int main_windows_managment();

// main() appart because INIReader uses RAII and don't get deleted before leak checking.
int main()
{
#ifdef WIN32
	//_CrtSetBreakAlloc(*place some value here to debug*);
#endif

	int result = main_windows_managment();

#ifdef WIN32
	_CrtDumpMemoryLeaks();
#endif

	return result;
}

int main_windows_managment()
{
	glfwSetErrorCallback(glfw_error_callback);

	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return -1;
	}


	if (DirExists("../../mass_collide/mass_collide_project/rc")){
		ChangeDir("../../mass_collide/mass_collide_project");
		std::cout << "found recource folder and changed directory\n";
	}
	if (DirExists("../mass_collide/mass_collide_project/rc")){
		ChangeDir("../mass_collide/mass_collide_project");
		std::cout << "found recource folder and changed directory\n";
	}
	LogCurrentDir();

	INIReader reader("rc/settings.ini");
	g_settings = &reader;

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
	TwWindowSize(iniWindowWidth, iniWindowHeight);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow(
		iniWindowWidth,
		iniWindowHeight,
		"mass_collide, simulations on the GPU --Emile Sonneveld", NULL, NULL);
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



	double dt;// Current time and enlapsed time
	float bgColor[] = { .0f, .0f, .4f };
	bool drawConnections = true;
	bool simulateConnections = false;

	glClearColor(bgColor[0], bgColor[1], bgColor[2], 1);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);


	// Un-used, but maby nedded for some pipeline stuff
	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);


	glfwSetCursorPos(window, 1024 / 2, 768 / 2);



	// Initialize AntTweakBar
	TwInit(TW_OPENGL_CORE, NULL);

	auto bar = TwNewBar("TweakBar");
	TwDefine(" GLOBAL help='This example shows how to integrate AntTweakBar with GLFW and OpenGL.' "); // Message added to the help bar.
	// RW : ReadWrite
	TwAddVarRW(bar, "drawConnections", TW_TYPE_BOOLCPP, &drawConnections, "label='drawConnections' key=C");
	TwAddVarRW(bar, "simulateConnections", TW_TYPE_BOOLCPP, &simulateConnections, "label='simulateConnections' key=V");
	// read-only (RO)
	TwAddVarRO(bar, "dt", TW_TYPE_DOUBLE, &dt, "label='dt'"); // precision=1 
	// TW_TYPE_COLOR3F (3 floats color)
	TwAddVarRW(bar, "bgColor", TW_TYPE_COLOR3F, &bgColor, "label='Background color'");


	// - Directly redirect GLFW events to AntTweakBar 
	glfwSetMouseButtonCallback(window, (GLFWmousebuttonfun)TwEventMouseButtonGLFW);
	glfwSetCursorPosCallback(window, (GLFWcursorposfun)TwEventMousePosGLFW);
	glfwSetScrollCallback(window, (GLFWscrollfun)TwEventMouseWheelGLFW);
	glfwSetKeyCallback(window, (GLFWkeyfun)TwEventKeyGLFW); // Wrong signature casting, Tw will not get the keyactions
	glfwSetCharCallback(window, (GLFWcharfun)TwEventCharGLFW);


	//////////////////////////////////////////////////////////////
	particle_data particle_data_inst;
	data data_inst;
	data_inst.doAllTheInitisation(particle_data_inst);


	c_particle_draw particle_draw;
	c_transform_feedback transform_positions;
	c_transform_feedback transform_velocities;
	c_connections_transform_feedback connection_force;
	c_connections_draw connections_draw;

	particle_draw.initialize();
	transform_positions.initialize("rc/compute.glsl", bufferName::position);
	transform_velocities.initialize("rc/forces.glsl", bufferName::velocity);
	connection_force.initialize("rc/connection_force.glsl", bufferName::velocity);
	connections_draw.initialize();
	//////////////////////////////////////////////////////////////


	double lastTime = glfwGetTime();



	bool isFirstTime = true;

	do
	{
		double currentTime = glfwGetTime();
		dt = float(currentTime - lastTime);
		lastTime = currentTime;

		glClearColor(bgColor[0], bgColor[1], bgColor[2], 1);
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
			transform_positions.process(particle_data_inst);
			transform_velocities.process(particle_data_inst);
			printOpenGLError();
		}
		if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS){
			particle_draw.clean();
			transform_positions.clean();
			transform_velocities.clean();
			connection_force.clean();
			connections_draw.clean();

			particle_draw.initialize();
			transform_positions.initialize("rc/compute.glsl", bufferName::position);
			transform_velocities.initialize("rc/forces.glsl", bufferName::velocity);
			connection_force.initialize("rc/connection_force.glsl", bufferName::velocity);
			connections_draw.initialize();
		}
		if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS){
			reader = INIReader("rc/settings.ini");
			data_inst.clean();
			data_inst = data();
			data_inst.doAllTheInitisation(particle_data_inst);
		}
		particle_draw.process(particle_data_inst);
		if (glfwGetKey(window, GLFW_KEY_C) != GLFW_PRESS && drawConnections){
			connections_draw.process(particle_data_inst);
			printOpenGLError();
		}
		if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS || simulateConnections){
			transform_positions.process(particle_data_inst);
			transform_velocities.process(particle_data_inst);
			connection_force.process(particle_data_inst);
			printOpenGLError();
		}
		//////////////////////////////////////////////////////////////

		TwDraw();

		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
	glfwWindowShouldClose(window) == 0);


	printOpenGLError();

	TwTerminate();
	//////////////////////////////////////////////////////////////
	particle_data_inst.clean();
	particle_draw.clean();
	transform_positions.clean();
	//////////////////////////////////////////////////////////////


	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	printOpenGLError();

	return 0;
}
