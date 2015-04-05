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
#include "particle_data.h"


void particle_system::initialize()
{
	// Create and compile our GLSL program from the shaders //
	//////////////////////////////////////////////////////////
	m_shader_program = LoadShaders("Particle.vertexshader", "Particle.fragmentshader");
	printOpenGLError();

	// Vertex shader
	m_uniform_CameraRight_worldspace = glGetUniformLocation(m_shader_program, "CameraRight_worldspace");
	printOpenGLError();
	m_uniform_CameraUp_worldspace = glGetUniformLocation(m_shader_program, "CameraUp_worldspace");
	m_uniform_ViewProjMatrix = glGetUniformLocation(m_shader_program, "ViewProjMatrix");
	// fragment shader
	m_uniform_TextureSampler = glGetUniformLocation(m_shader_program, "TextureSampler");


	m_in_attrib_square = glGetAttribLocation(m_shader_program, "squareVertices");
	m_in_attrib_position = glGetAttribLocation(m_shader_program, "inPosition");
	m_in_attrib_color = glGetAttribLocation(m_shader_program, "inColor");


	m_texture = loadDDS("particle.DDS");

	printOpenGLError();

	// Create and fill in the graphical mesh //
	///////////////////////////////////////////
	static const GLfloat g_vertex_buffer_data[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		-0.5f, 0.5f, 0.0f,
		0.5f, 0.5f, 0.0f,
	};
	glGenBuffers(1, &m_buffer_billboard_vertex);
	glBindBuffer(GL_ARRAY_BUFFER, m_buffer_billboard_vertex);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	printOpenGLError();
}

void particle_system::draw(particle_data& particle_data_ref)
{
	// fill in the shader uniforms //
	/////////////////////////////////
	glUseProgram(m_shader_program);


	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glUniform1i(m_uniform_TextureSampler, 0); // to user Texture Unit 0

	glm::mat4 ProjectionMatrix = getProjectionMatrix();
	glm::mat4 ViewMatrix = getViewMatrix();
	glm::mat4 ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;

	glUniform3f(m_uniform_CameraRight_worldspace, ViewMatrix[0][0], ViewMatrix[1][0], ViewMatrix[2][0]);
	glUniform3f(m_uniform_CameraUp_worldspace, ViewMatrix[0][1], ViewMatrix[1][1], ViewMatrix[2][1]);

	glUniformMatrix4fv(m_uniform_ViewProjMatrix, 1, GL_FALSE, &ViewProjectionMatrix[0][0]);

	// Draw instanced //
	////////////////////

	glEnable(GL_DEPTH_TEST);

	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(m_in_attrib_square);
	glBindBuffer(GL_ARRAY_BUFFER, m_buffer_billboard_vertex);
	glVertexAttribPointer(
		m_in_attrib_square,                  // attribute. Must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0             // array buffer offset
		);

	glEnableVertexAttribArray(m_in_attrib_position);
	glBindBuffer(GL_ARRAY_BUFFER, particle_data_ref.m_buffer_position);
	glVertexAttribPointer(
		m_in_attrib_position,
		4,
		GL_FLOAT,
		GL_FALSE,
		0,
		nullptr
		);

	glEnableVertexAttribArray(m_in_attrib_color);
	glBindBuffer(GL_ARRAY_BUFFER, particle_data_ref.m_buffer_color);
	glVertexAttribPointer(
		m_in_attrib_color,
		4,
		GL_FLOAT, //GL_UNSIGNED_BYTE,
		GL_FALSE,             // YES, this means that the unsigned char[4] will be accessible with a vec4 (floats) in the shader
		0,
		nullptr
		);

	// These functions are specific to glDrawArrays*Instanced*.
	// The first parameter is the attribute buffer we're talking about.
	// The second parameter is the "rate at which generic vertex attributes advance when rendering multiple instances"
	// http://www.opengl.org/sdk/docs/man/xhtml/glVertexAttribDivisor.xml
	glVertexAttribDivisor(m_in_attrib_square, 0); // particles vertices : always reuse the same 4 vertices -> 0
	glVertexAttribDivisor(m_in_attrib_position, 1); // positions : one per quad (its center)                 -> 1
	glVertexAttribDivisor(m_in_attrib_color, 1); // color : one per quad                                  -> 1

	printOpenGLError();

	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, particle_data::COUNT);

	glVertexAttribDivisor(m_in_attrib_square, 0);
	glVertexAttribDivisor(m_in_attrib_position, 0);
	glVertexAttribDivisor(m_in_attrib_color, 0);

	glDisableVertexAttribArray(m_in_attrib_square);
	glDisableVertexAttribArray(m_in_attrib_position);
	glDisableVertexAttribArray(m_in_attrib_color);

	printOpenGLError(); // !
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindTexture(GL_TEXTURE_2D, 0);

}

void particle_system::clean()
{
	if (m_buffer_billboard_vertex)
		glDeleteBuffers(1, &m_buffer_billboard_vertex);
	m_buffer_billboard_vertex = 0;

	if (m_shader_program)
		glDeleteProgram(m_shader_program);
	m_shader_program = 0;

	if (m_texture)
		glDeleteTextures(1, &m_texture);
	m_texture = 0;
}
