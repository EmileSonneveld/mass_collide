
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

#include "globals.h"
#include "particle_data.h"
#include "particle_system.h"
#include "transform_feedback.h"


int main(void)
{
	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // was 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow(1024, 768, "Tutorial 18 - Particules", NULL, NULL);
	if (window == NULL){
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	glfwSetCursorPos(window, 1024 / 2, 768 / 2);
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
	initialize_buffers(particle_data_inst);
	initialize_velocity(particle_data_inst);

	transform_feedback transform_positions;
	transform_positions.initialize("compute.glsl");

	transform_feedback transform_velocities;
	transform_velocities.initialize("forces.glsl");
	//////////////////////////////////////////////////////////////
	bool isFirstTime = true;

	double lastTime = glfwGetTime();
	do
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		printOpenGLError();
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		if (!isFirstTime)
			computeMatricesFromInputs();
		isFirstTime = false;

		//////////////////////////////////////////////////////////////
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
			transform_positions.ProccesPositions(particle_data_inst);
			transform_velocities.ProccesVelocities(particle_data_inst);
		}
		if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS){
			transform_positions.initialize("compute.glsl");
			transform_velocities.initialize("forces.glsl");
		}
		if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS){
			initialize_buffers(particle_data_inst);
		}
		particle_system_inst.draw(particle_data_inst);
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

	return 0;
}
