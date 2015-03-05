#include "transform_feedback.h"

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


#include "globals.h"

//#define TESTCASE


GLint CompileComputeShader(const GLchar* filename)
{
	printOpenGLError();

	GLuint shader = glCreateShader(GL_VERTEX_SHADER);
	auto shader_compute = getCodeFromFile(filename);
	char const * shader_compute_pointer = shader_compute.c_str();
	glShaderSource(shader, 1, &shader_compute_pointer, nullptr);
	glCompileShader(shader);
	printOpenGLError();

	GLuint program = glCreateProgram();
	glAttachShader(program, shader);

	printOpenGLError();
	// transform feedback specefic
	const GLchar* feedbackVaryings[] = { "outValue" };
	// or GL_SEPARATE_ATTRIBS GL_INTERLEAVED_ATTRIBS
	glTransformFeedbackVaryings(program, 1, feedbackVaryings, GL_INTERLEAVED_ATTRIBS);

	printOpenGLError();

	glLinkProgram(program);
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
	// GLint inputAttrib = glGetAttribLocation(program, "inValue");
	// glEnableVertexAttribArray(inputAttrib);
	// glVertexAttribPointer(inputAttrib, 1, GL_FLOAT, GL_FALSE, 0, 0);

	return program;
}


void transform_feedback::SetSourceDataAndGenerateSwapBuffer(GLint source)
{
#ifndef TESTCASE
	m_buffer_input = source;

	glBindBuffer(GL_ARRAY_BUFFER, source);
	glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &m_buffer_size);
	// int originalVertexArraySize = (nBufferSize / sizeof(float));

	// resutlt VBO
	glGenBuffers(1, &m_buffer_result);
	glBindBuffer(GL_ARRAY_BUFFER, m_buffer_result);
	glBufferData(GL_ARRAY_BUFFER, m_buffer_size, nullptr, GL_DYNAMIC_DRAW); //GL_DYNAMIC_DRAW GL_STATIC_READ

	int nBufferSize2 = 0; // check value
	glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &nBufferSize2);
	assert(nBufferSize2 == m_buffer_size);

	auto nr_of_particles = m_buffer_size / sizeof(glm::vec4);

	std::default_random_engine generator;
	std::normal_distribution<float> distribution(5.0, 2.0);

	auto* data_random_vel = new vec4[nr_of_particles];
	for (unsigned int i = 0; i < nr_of_particles; ++i){
		data_random_vel[i].x = distribution(generator);
		data_random_vel[i].y = distribution(generator);
		data_random_vel[i].z = distribution(generator);
		data_random_vel[i].a = 0; // padding
	}

	glGenBuffers(1, &m_buffer_velocity);
	glBindBuffer(GL_ARRAY_BUFFER, m_buffer_velocity);
	glBufferData(GL_ARRAY_BUFFER, m_buffer_size, data_random_vel, GL_DYNAMIC_DRAW);

	delete data_random_vel;

#endif
}


void transform_feedback::initialize()
{
	printOpenGLError();

	if (m_program != 0){
		glDeleteProgram(m_program);
		m_program = 0;
	}
	m_program = CompileComputeShader("compute.glsl");





#ifdef TESTCASE
	GLfloat data[] = { 1.0f, 2.0f, 3.0f, 4.0f, 5.0f };  // demo data

	glGenBuffers(1, &m_buffer_input);
	glBindBuffer(GL_ARRAY_BUFFER, m_buffer_input);
	glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);

	// resutlt VBO gets generated from source buffer
	glGenBuffers(1, &m_buffer_result);
	glBindBuffer(GL_ARRAY_BUFFER, m_buffer_result);
	glBufferData(GL_ARRAY_BUFFER, sizeof(data), nullptr, GL_STATIC_READ);
#endif

	printOpenGLError();
}

GLint transform_feedback::DoTheCalculation()
{
	// gives more misery and has no visible efect when disabled
	//glEnable(GL_RASTERIZER_DISCARD); 
	glUseProgram(m_program);

	printOpenGLError();
	// bind our buffer to the Transform feedback
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, m_buffer_result);

	printOpenGLError();

	glEnableVertexAttribArray(m_in_attrib_position);
	glBindBuffer(GL_ARRAY_BUFFER, m_buffer_input);
	glVertexAttribPointer(m_in_attrib_position, 4, GL_FLOAT, GL_FALSE, 0, 0);

	glEnableVertexAttribArray(m_in_attrib_velocity);
	glBindBuffer(GL_ARRAY_BUFFER, m_buffer_velocity);
	glVertexAttribPointer(m_in_attrib_velocity, 4, GL_FLOAT, GL_FALSE, 0, 0);

	// glVertexAttribDivisor(0, 1);
	// glVertexAttribDivisor(1, 1);

	// GL_POINTS — GL_POINTS
	// GL_LINES — GL_LINES, GL_LINE_LOOP, GL_LINE_STRIP, GL_LINES_ADJACENCY, GL_LINE_STRIP_ADJACENCY
	// GL_TRIANGLES — GL_TRIANGLES, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN, GL_TRIANGLES_ADJACENCY, GL_TRIANGLE_STRIP_ADJACENCY
	glBeginTransformFeedback(GL_POINTS);
	glDrawArrays(GL_POINTS, 0, m_buffer_size);
	glEndTransformFeedback();


	glFlush();


#ifdef TESTCASE
	// get the data back to CPU
	GLfloat* feedback = new GLfloat[m_buffer_size];
	glGetBufferSubData(GL_TRANSFORM_FEEDBACK_BUFFER, 0, m_buffer_size * sizeof(GLfloat), feedback);
	printf("%f %f %f %f %f\n", feedback[0], feedback[1], feedback[2], feedback[3], feedback[4]);
	//1.000000 1.414214 1.732051 2.000000 2.236068
#endif

	auto tmp_result = m_buffer_result;
	m_buffer_result = m_buffer_input;
	m_buffer_input = tmp_result;

	return tmp_result;
}

void transform_feedback::clean()
{
	if (m_program != 0)
		glDeleteProgram(m_program);
	glDeleteBuffers(1, &m_buffer_velocity);
}
