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
	glGenTextures(1, &m_texture_buffer_OtherIndex);
	glGenTextures(1, &m_texture_buffer_LengthToOther);

	m_program = LoadShaderWithTransformFeedback(file_name.c_str());
	m_uniform_samplerPosition = glGetUniformLocation(m_program, "samplerPosition");
	m_uniform_samplerOtherIndex = glGetUniformLocation(m_program, "samplerOtherIndex");
	m_uniform_samplerLengthToOther = glGetUniformLocation(m_program, "samplerLengthToOther");
	m_in_attrib_position = glGetAttribLocation(m_program, "inPosition");
	m_in_attrib_velocity = glGetAttribLocation(m_program, "inVelocity");
}

void c_connections_transform_feedback::process(particle_data& particle_data_ref)
{
	if (particle_data_ref.CONNECTION_COUNT == 0)
	{
		// there is nothing to do here if there are no connections
		return;
	}
	glUseProgram(m_program);

	glEnable(GL_RASTERIZER_DISCARD);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, particle_data_ref.buffer[swap]);

	printOpenGLError();

	if (m_in_attrib_position != -1){
		glEnableVertexAttribArray(m_in_attrib_position);
		glBindBuffer(GL_ARRAY_BUFFER, particle_data_ref.buffer[position]);
		glVertexAttribPointer(m_in_attrib_position, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
	}
	if (m_in_attrib_velocity != -1){
		glEnableVertexAttribArray(m_in_attrib_velocity);
		glBindBuffer(GL_ARRAY_BUFFER, particle_data_ref.buffer[velocity]);
		glVertexAttribPointer(m_in_attrib_velocity, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
	}
	printOpenGLError();

	////////////////////////////////////////////////////////////////////////

	glActiveTexture(GL_TEXTURE0 + 0);
	glBindTexture(GL_TEXTURE_BUFFER, m_texture_buffer);
	//glBindSampler is irrelavant
	glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, particle_data_ref.buffer[position]);
	glUniform1i(m_uniform_samplerPosition, 0); // to user Texture Unit

	glActiveTexture(GL_TEXTURE0 + 1);
	glBindTexture(GL_TEXTURE_BUFFER, m_texture_buffer_OtherIndex);
	//glBindSampler is irrelavant
	glTexBuffer(GL_TEXTURE_BUFFER, GL_R32I, particle_data_ref.buffer[connection_index_alt]);
	glUniform1i(m_uniform_samplerOtherIndex, 1); // to user Texture Unit

	glActiveTexture(GL_TEXTURE0 + 2);
	glBindTexture(GL_TEXTURE_BUFFER, m_texture_buffer_LengthToOther);
	//glBindSampler is irrelavant
	glTexBuffer(GL_TEXTURE_BUFFER, GL_R32F, particle_data_ref.buffer[connection_length_alt]);
	glUniform1i(m_uniform_samplerLengthToOther, 2); // to user Texture Unit

	printOpenGLError();
	////////////////////////////////////////////////////////////////////////

	glBeginTransformFeedback(GL_POINTS);

	glDrawArrays(GL_POINTS, 0, particle_data_ref.COUNT);

	// glVertexAttribDivisor(m_in_attrib_otherIndex, 4);
	// glVertexAttribDivisor(m_in_attrib_lengthToOther, 4);
	// glDrawArraysInstanced(GL_POINTS, 0, 4, particle_data_ref.COUNT);
	// glVertexAttribDivisor(m_in_attrib_otherIndex, 0);
	// glVertexAttribDivisor(m_in_attrib_lengthToOther, 0);

	glEndTransformFeedback();

	glBindTexture(GL_TEXTURE_2D, 0);

	glDisable(GL_RASTERIZER_DISCARD);

	if (m_in_attrib_position != -1){
		glDisableVertexAttribArray(m_in_attrib_position);
	}
	glDisableVertexAttribArray(m_in_attrib_velocity);

	printOpenGLError();
	glFlush();

	printOpenGLError();
	auto tmp_result = particle_data_ref.buffer[swap];
	particle_data_ref.buffer[swap] = particle_data_ref.buffer[m_transform_feedback_out];
	particle_data_ref.buffer[m_transform_feedback_out] = tmp_result;

	printf("vel after connection_force.glsl \n");
	printTransformFeedbackValues(min(5U, particle_data_ref.COUNT));
	printOpenGLError();
}

void c_connections_transform_feedback::clean()
{
	if (m_program)
		glDeleteProgram(m_program);
	m_program = 0;
}
