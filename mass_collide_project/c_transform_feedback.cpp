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


GLint CompileComputeShader(const GLchar* filename)
{
	printOpenGLError();
	GLuint shader = LoadTemporaryShader(filename, GL_VERTEX_SHADER);

	GLuint program = glCreateProgram();
	glAttachShader(program, shader);

	printOpenGLError();
	// transform feedback specefic
	const GLchar* feedbackVaryings[] = { "outValue" };
	// or GL_SEPARATE_ATTRIBS GL_INTERLEAVED_ATTRIBS
	glTransformFeedbackVaryings(program, 1, feedbackVaryings, GL_INTERLEAVED_ATTRIBS);

	printOpenGLError();

	glLinkProgram(program);
    glValidateProgram(program);

	// Check the program
	{
		GLint Result = GL_FALSE;
		int InfoLogLength;
		glGetProgramiv(program, GL_LINK_STATUS, &Result);
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &InfoLogLength);
		if (InfoLogLength > 0){
			std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
			glGetProgramInfoLog(program, InfoLogLength, NULL, &ProgramErrorMessage[0]);
			printf("%s\n", &ProgramErrorMessage[0]);
		}
	}
	glUseProgram(program);

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	printOpenGLError();

	return program;
}

void c_transform_feedback::initialize(std::string file_name, bufferName output_buffer_name)
{
	m_transform_feedback_out = output_buffer_name;

	m_program = CompileComputeShader(file_name.c_str());
	m_uniform_point = glGetUniformLocation(m_program, "point");
	m_in_attrib_position = glGetAttribLocation(m_program, "inPosition");
	m_in_attrib_velocity = glGetAttribLocation(m_program, "inVelocity");
}

void c_transform_feedback::process(particle_data& particle_data_ref) // ProccesPositions
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

void c_transform_feedback::clean()
{
	if (m_program != 0)
		glDeleteProgram(m_program);
	m_program = 0;
}
