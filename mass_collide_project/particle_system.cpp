#include "particle_system.h"

#include <stdio.h>
#include <stdlib.h>

#include <vector>
#include <algorithm>
#include <string>
#include <iostream>
#include <fstream>

#include <GL/glew.h>

#include <glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/norm.hpp>
using namespace glm;

#include <common/shader.hpp>
#include <common/texture.hpp>
#include <common/controls.hpp>


#include "globals.h"
#include "transform_feedback.h"


void particle_system::initialize()
{

	// Create and compile our GLSL program from the shaders //
	//////////////////////////////////////////////////////////
	m_shader_program = LoadShaders("Particle.vertexshader", "Particle.fragmentshader");

	// Vertex shader
	m_uniform_CameraRight_worldspace = glGetUniformLocation(m_shader_program, "CameraRight_worldspace");
	m_uniform_CameraUp_worldspace = glGetUniformLocation(m_shader_program, "CameraUp_worldspace");
	m_uniform_ViewProjMatrix = glGetUniformLocation(m_shader_program, "ViewProjMatrix");
	// fragment shader
	m_uniform_TextureSampler = glGetUniformLocation(m_shader_program, "TextureSampler");

	// m_compute_program = LoadComputeShader("Particle.computeshader"); // TODO 
	printOpenGLError();

	m_texture = loadDDS("particle.DDS");



	printOpenGLError();
	// single serving data to init GPU stuff //
	///////////////////////////////////////////

	auto* data_random_pos = new vec4[PARTICLES_COUNT];
	auto* data_random_col = new GLubyte[PARTICLES_COUNT * 4];

	for (unsigned int i = 0; i < PARTICLES_COUNT; ++i){
		data_random_pos[i].x = (float)(rand() % 100) / 10.0f;
		data_random_pos[i].y = (float)(rand() % 100) / 10.0f;
		data_random_pos[i].z = (float)(rand() % 100) / 10.0f;
		data_random_pos[i].a = 0.02f; // size

		data_random_col[i + 0] = rand() % 128;
		data_random_col[i + 1] = rand() % 128;
		data_random_col[i + 2] = rand() % 128;
		data_random_col[i + 3] = 128;


		//data_random_pos[i].x = i % 100;
		//data_random_pos[i].y = i / 100 % 100;
		//data_random_pos[i].z = i / 100 % 100 % 100;
	}


	printOpenGLError();
	// Create and initialize the Buffer Objects on the GPU //
	//////////////////////////////////////////////////

	glGenBuffers(1, &m_buffer_position);
	glBindBuffer(GL_ARRAY_BUFFER, m_buffer_position);
	glBufferData(GL_ARRAY_BUFFER, PARTICLES_COUNT * sizeof(vec4), data_random_pos, GL_DYNAMIC_DRAW);

	glGenBuffers(1, &m_buffer_color);
	glBindBuffer(GL_ARRAY_BUFFER, m_buffer_color);
	glBufferData(GL_ARRAY_BUFFER, PARTICLES_COUNT * 4 * sizeof(GLubyte), data_random_col, GL_DYNAMIC_DRAW);


	delete data_random_pos;
	delete data_random_col;


	printOpenGLError();
	// Create and fill in the graphical mesh //
	///////////////////////////////////////////

	// The VBO containing the 4 vertices of the particles.
	// Thanks to instancing, they will be shared by all particles.
	static const GLfloat g_vertex_buffer_data[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		-0.5f, 0.5f, 0.0f,
		0.5f, 0.5f, 0.0f,
	};
	glGenBuffers(1, &m_buffer_billboard_vertex);
	glBindBuffer(GL_ARRAY_BUFFER, m_buffer_billboard_vertex);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);



	glBindBuffer(GL_ARRAY_BUFFER, 0); // unbind


	printOpenGLError();
	m_transform_feedback_inst.initialize();
	m_transform_feedback_inst.SetSourceDataAndGenerateSwapBuffer(m_buffer_position);

}

void particle_system::tick()
{
	printOpenGLError();


	m_buffer_position = m_transform_feedback_inst.DoTheCalculation();

	// fill in the shader uniforms //
	/////////////////////////////////
	glUseProgram(m_shader_program);


	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_texture);
	// Set our "TextureSampler" sampler to user Texture Unit 0
	glUniform1i(m_uniform_TextureSampler, 0);


	glm::mat4 ProjectionMatrix = getProjectionMatrix();
	glm::mat4 ViewMatrix = getViewMatrix();
	glm::mat4 ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;

	glUniform3f(m_uniform_CameraRight_worldspace, ViewMatrix[0][0], ViewMatrix[1][0], ViewMatrix[2][0]);
	glUniform3f(m_uniform_CameraUp_worldspace, ViewMatrix[0][1], ViewMatrix[1][1], ViewMatrix[2][1]);

	glUniformMatrix4fv(m_uniform_ViewProjMatrix, 1, GL_FALSE, &ViewProjectionMatrix[0][0]);



	printOpenGLError();
	// Draw instanced //
	////////////////////

	glEnable(GL_DEPTH_TEST);

	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, m_buffer_billboard_vertex);
	glVertexAttribPointer(
		0,                  // attribute. Must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0             // array buffer offset
		);

	printOpenGLError(); // !
	// 2nd attribute buffer : positions of particles' centers
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, m_buffer_position);
	glVertexAttribPointer(
		1,                  // attribute. Must match the layout in the shader.
		4,                  // size : x + y + z + size => 4
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		nullptr             // array buffer offset
		);

	// 3rd attribute buffer : particles' colors
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, m_buffer_color);
	glVertexAttribPointer(
		2,                   // attribute. Must match the layout in the shader.
		4,                   // size : r + g + b + a => 4
		GL_UNSIGNED_BYTE,    // type
		GL_TRUE,             // normalized?    *** YES, this means that the unsigned char[4] will be accessible with a vec4 (floats) in the shader ***
		0,                   // stride
		nullptr              // array buffer offset
		);

	// These functions are specific to glDrawArrays*Instanced*.
	// The first parameter is the attribute buffer we're talking about.
	// The second parameter is the "rate at which generic vertex attributes advance when rendering multiple instances"
	// http://www.opengl.org/sdk/docs/man/xhtml/glVertexAttribDivisor.xml
	glVertexAttribDivisor(0, 0); // particles vertices : always reuse the same 4 vertices -> 0
	glVertexAttribDivisor(1, 1); // positions : one per quad (its center)                 -> 1
	glVertexAttribDivisor(2, 1); // color : one per quad                                  -> 1

	printOpenGLError(); // !

	// This draws many times a small triangle_strip (which looks like a quad).
	// This is equivalent to :
	// for(i in ParticlesCount) : glDrawArrays(GL_TRIANGLE_STRIP, 0, 4),
	// but faster.
	//glDrawArrays(GL_QUADS, 0, 4);
	//for (unsigned int i = 0; i < PARTICLES_COUNT; ++i) { glDrawArrays(GL_TRIANGLE_STRIP, 0, 4); }
	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, PARTICLES_COUNT);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	printOpenGLError(); // !
	glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void particle_system::clean()
{
	glDeleteBuffers(1, &m_buffer_billboard_vertex);
	glDeleteBuffers(1, &m_buffer_position);
	glDeleteBuffers(1, &m_buffer_color);

	glDeleteProgram(m_shader_program);
	glDeleteTextures(1, &m_texture);

}
