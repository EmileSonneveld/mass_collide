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

#include "particle_data.h"
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




void transform_feedback::initialize()
{
	printOpenGLError();
	clean();
	m_program = CompileComputeShader("compute.glsl");

	printOpenGLError();
}

GLint transform_feedback::DoTheCalculation(particle_data& particle_data_ref)
{
	// gives more misery and has no visible efect when disabled
	//glEnable(GL_RASTERIZER_DISCARD); 
	glUseProgram(m_program);

	printOpenGLError();
	// bind our buffer to the Transform feedback
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, particle_data_ref.m_buffer_position_swap);

	printOpenGLError();

	glEnableVertexAttribArray(m_in_attrib_position);
	glBindBuffer(GL_ARRAY_BUFFER, particle_data_ref.m_buffer_position);
	glVertexAttribPointer(m_in_attrib_position, 4, GL_FLOAT, GL_FALSE, 0, nullptr);

	glEnableVertexAttribArray(m_in_attrib_velocity);
	glBindBuffer(GL_ARRAY_BUFFER, particle_data_ref.m_buffer_velocity);
	glVertexAttribPointer(m_in_attrib_velocity, 4, GL_FLOAT, GL_FALSE, 0, nullptr);

	//glVertexAttribDivisor(0, 1);
	//texAttribDivisor(1, 1);
	//texAttribDivisor(2, 1);

	// GL_POINTS — GL_POINTS
	// GL_LINES — GL_LINES, GL_LINE_LOOP, GL_LINE_STRIP, GL_LINES_ADJACENCY, GL_LINE_STRIP_ADJACENCY
	// GL_TRIANGLES — GL_TRIANGLES, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN, GL_TRIANGLES_ADJACENCY, GL_TRIANGLE_STRIP_ADJACENCY
	glBeginTransformFeedback(GL_POINTS);
	glDrawArrays(GL_POINTS, 0, particle_data::COUNT);
	//glDrawArraysInstanced(GL_POINTS, 0, 4, particle_data::COUNT);
	glEndTransformFeedback();


	printOpenGLError();
	// get the data back to CPU
	auto nr_catch_particles = 1;
	vec4* feedback = new vec4[nr_catch_particles];
	glGetBufferSubData(GL_TRANSFORM_FEEDBACK_BUFFER, 0, nr_catch_particles * sizeof(vec4), feedback);
	printf("feedback: %f %f %f %f\n", feedback[0].x, feedback[0].y, feedback[0].z, feedback[0].a);
	printOpenGLError();

	glDisableVertexAttribArray(m_in_attrib_position);
	glDisableVertexAttribArray(m_in_attrib_velocity);

	glFlush();


#ifdef TESTCASE
	// get the data back to CPU
	GLfloat* feedback = new GLfloat[m_buffer_size];
	glGetBufferSubData(GL_TRANSFORM_FEEDBACK_BUFFER, 0, m_buffer_size * sizeof(GLfloat), feedback);
	printf("%f %f %f %f %f\n", feedback[0], feedback[1], feedback[2], feedback[3], feedback[4]);
	//1.000000 1.414214 1.732051 2.000000 2.236068
#endif

	auto tmp_result = particle_data_ref.m_buffer_position_swap;
	particle_data_ref.m_buffer_position_swap = particle_data_ref.m_buffer_position;
	particle_data_ref.m_buffer_position = tmp_result;

	return tmp_result;
}

void transform_feedback::clean()
{
	if (m_program != 0)
		glDeleteProgram(m_program);
	m_program = 0;
}
