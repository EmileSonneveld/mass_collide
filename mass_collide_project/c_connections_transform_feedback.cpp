#include "c_connections_transform_feedback.h"

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <string>
#include <iostream>
#include <fstream>
#include <assert.h>
#include <random>

#include <GL/glew.h>

#include <glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/norm.hpp>
using namespace glm;

#include <common/shader.hpp>
#include <common/texture.hpp>
#include <common/controls.hpp>

#include "particle_data.h"
#include "globals.h"



void c_connections_transform_feedback::initialize(std::string file_name, bufferName output_buffer_name)
{
	m_transform_feedback_out = output_buffer_name;

	glGenTextures(1, &m_texture_buffer);

	m_program = LoadShaderWithTransformFeedback(file_name.c_str());
	m_uniform_samplerPosition = glGetUniformLocation(m_program, "samplerPosition");
	m_uniform_samplerVelocity = glGetUniformLocation(m_program, "samplerVelocity");
	m_in_attrib_position = glGetAttribLocation(m_program, "inPosition");
	m_in_attrib_velocity = glGetAttribLocation(m_program, "inVelocity");
}

void c_connections_transform_feedback::process(particle_data& particle_data_ref)
{
	glUseProgram(m_program);

    glEnable(GL_RASTERIZER_DISCARD);
	// bind our buffer to the Transform feedback
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, particle_data_ref.buffer[swap]);

	printOpenGLError();

	glEnableVertexAttribArray(m_in_attrib_position);
	glBindBuffer(GL_ARRAY_BUFFER, particle_data_ref.buffer[position]);
	glVertexAttribPointer(m_in_attrib_position, 4, GL_FLOAT, GL_FALSE, 0, nullptr);

	glEnableVertexAttribArray(m_in_attrib_velocity);
	glBindBuffer(GL_ARRAY_BUFFER, particle_data_ref.buffer[velocity]);
	glVertexAttribPointer(m_in_attrib_velocity, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
	printOpenGLError();

	////////////////////////////////////////////////////////////////////////

	glActiveTexture(GL_TEXTURE0 + 0);
	glBindTexture(GL_TEXTURE_2D, m_texture_buffer);
	//glBindSampler is irrelavant

	//glBindBuffer(GL_UNIFORM_BUFFER, particle_data_ref.buffer[position]);
	glBindBuffer(GL_TEXTURE_BUFFER, particle_data_ref.buffer[velocity]);
	glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, particle_data_ref.buffer[velocity]);

	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, m_texture_buffer);
	glUniform1i(m_uniform_samplerVelocity, 0); // to user Texture Unit 0
	
	// GL_MAX_TEXTURE_BUFFER_SIZE is mininum 65536

	////////////////////////////////////////////////////////////////////////

	// GL_POINTS
	// GL_LINES, GL_LINE_LOOP, GL_LINE_STRIP, GL_LINES_ADJACENCY, GL_LINE_STRIP_ADJACENCY
	// GL_TRIANGLES, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN, GL_TRIANGLES_ADJACENCY, GL_TRIANGLE_STRIP_ADJACENCY
	glBeginTransformFeedback(GL_POINTS);
	glDrawArrays(GL_POINTS, 0, particle_data_ref.COUNT);
	glEndTransformFeedback();

    glDisable(GL_RASTERIZER_DISCARD);

	glDisableVertexAttribArray(m_in_attrib_position);
	glDisableVertexAttribArray(m_in_attrib_velocity);

	printOpenGLError();
	glFlush();

	printOpenGLError();
	auto tmp_result = particle_data_ref.buffer[swap];
	particle_data_ref.buffer[swap] = particle_data_ref.buffer[m_transform_feedback_out];
	particle_data_ref.buffer[m_transform_feedback_out] = tmp_result;
}

void c_connections_transform_feedback::clean()
{
	if (m_program != 0)
		glDeleteProgram(m_program);
	m_program = 0;
}
