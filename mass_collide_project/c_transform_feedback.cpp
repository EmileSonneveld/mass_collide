#include "c_transform_feedback.h"

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

#include "common/shader.hpp"
#include "common/texture.hpp"
#include "common/controls.hpp"
#include "particle_data.h"
#include "globals.h"


using namespace glm;

void c_transform_feedback::initialize(const std::string& file_name, const bufferName output_buffer_name, const particle_data& particle_data_ref)
{
	m_transform_feedback_out = output_buffer_name;

	m_program = LoadShaderWithTransformFeedbackFromPath(file_name.c_str());
	m_uniform_point = glGetUniformLocation(m_program, "point");
	m_in_attrib_position = glGetAttribLocation(m_program, "inPosition");
	m_in_attrib_velocity = glGetAttribLocation(m_program, "inVelocity");
}

void c_transform_feedback::process(particle_data& particle_data_ref)
{
	glUseProgram(m_program);

	glEnable(GL_RASTERIZER_DISCARD);
	// bind our buffer to the Transform feedback
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, particle_data_ref.buffer[swap]);

	if (m_uniform_point != -1)
	{
		if (GetPsSetting_Bool("enable_mouse_input", true))
		{
			glm::vec4 pos = CursorToWorldspace(0.5);
			glUniform3f(m_uniform_point, pos.x, pos.y, pos.z);
		}
		else {
			glUniform3f(m_uniform_point, particle_data_init::EMPTY_VALUE, particle_data_init::EMPTY_VALUE, particle_data_init::EMPTY_VALUE);
		}
	}

	printOpenGLError();

	if (m_in_attrib_position != -1) {
		glEnableVertexAttribArray(m_in_attrib_position);
		glBindBuffer(GL_ARRAY_BUFFER, particle_data_ref.buffer[position]);
		glVertexAttribPointer(m_in_attrib_position, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
	}
	if (m_in_attrib_velocity != -1) {
		glEnableVertexAttribArray(m_in_attrib_velocity);
		glBindBuffer(GL_ARRAY_BUFFER, particle_data_ref.buffer[velocity]);
		glVertexAttribPointer(m_in_attrib_velocity, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
	}

	printOpenGLError();

	// GL_POINTS
	// GL_LINES, GL_LINE_LOOP, GL_LINE_STRIP, GL_LINES_ADJACENCY, GL_LINE_STRIP_ADJACENCY
	// GL_TRIANGLES, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN, GL_TRIANGLES_ADJACENCY, GL_TRIANGLE_STRIP_ADJACENCY
	glBeginTransformFeedback(GL_POINTS);
	glDrawArrays(GL_POINTS, 0, particle_data_ref.COUNT);
	glEndTransformFeedback();

	glDisable(GL_RASTERIZER_DISCARD);

	if (m_in_attrib_position != -1)
		glDisableVertexAttribArray(m_in_attrib_position);
	if (m_in_attrib_velocity != -1)
		glDisableVertexAttribArray(m_in_attrib_velocity);

	printOpenGLError();
	glFlush();

	printOpenGLError();
	auto tmp_result = particle_data_ref.buffer[swap];
	particle_data_ref.buffer[swap] = particle_data_ref.buffer[m_transform_feedback_out];
	particle_data_ref.buffer[m_transform_feedback_out] = tmp_result;

	//printTransformFeedbackValues(min(5U, particle_data_ref.COUNT));
}

void c_transform_feedback::clean()
{
	if (m_program)
		glDeleteProgram(m_program);
	m_program = 0U;
}
