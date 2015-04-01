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


GLint CompileComputeShader(const GLchar* filename)
{
	printOpenGLError();
	GLuint shader = LoadTemporaryShader(filename, GL_VERTEX_SHADER);

	// GLuint shader = glCreateShader(GL_VERTEX_SHADER);
	// auto shader_compute = getCodeFromFile(filename);
	// char const * shader_compute_pointer = shader_compute.c_str();
	// glShaderSource(shader, 1, &shader_compute_pointer, nullptr);
	// glCompileShader(shader);
	// printOpenGLError();
	// 
	// 
	// // Check Vertex Shader
	// GLint Result = GL_FALSE;
	// int InfoLogLength;
	// glGetShaderiv(shader, GL_COMPILE_STATUS, &Result);
	// glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &InfoLogLength);
	// if (InfoLogLength > 0){
	// 	std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
	// 	glGetShaderInfoLog(shader, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
	// 	printf("%s\n", &VertexShaderErrorMessage[0]);
	// }

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

	return program;
}

void transform_feedback::initialize(std::string file_name)
{
	clean();
	m_program = CompileComputeShader(file_name.c_str());
	m_uniform_point = glGetUniformLocation(m_program, "point");
}

GLint transform_feedback::ProccesPositions(particle_data& particle_data_ref)
{
	// gives more misery and has no visible efect when disabled
	//glEnable(GL_RASTERIZER_DISCARD); 
	glUseProgram(m_program);

	// bind our buffer to the Transform feedback
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, particle_data_ref.m_buffer_swap);

	printOpenGLError();

	glEnableVertexAttribArray(m_in_attrib_position);
	glBindBuffer(GL_ARRAY_BUFFER, particle_data_ref.m_buffer_position);
	glVertexAttribPointer(m_in_attrib_position, 4, GL_FLOAT, GL_FALSE, 0, nullptr);

	glEnableVertexAttribArray(m_in_attrib_velocity);
	glBindBuffer(GL_ARRAY_BUFFER, particle_data_ref.m_buffer_velocity);
	glVertexAttribPointer(m_in_attrib_velocity, 4, GL_FLOAT, GL_FALSE, 0, nullptr);

	// GL_POINTS
	// GL_LINES, GL_LINE_LOOP, GL_LINE_STRIP, GL_LINES_ADJACENCY, GL_LINE_STRIP_ADJACENCY
	// GL_TRIANGLES, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN, GL_TRIANGLES_ADJACENCY, GL_TRIANGLE_STRIP_ADJACENCY
	glBeginTransformFeedback(GL_POINTS);
	glDrawArrays(GL_POINTS, 0, particle_data::COUNT);
	glEndTransformFeedback();

	glDisableVertexAttribArray(m_in_attrib_position);
	glDisableVertexAttribArray(m_in_attrib_velocity);

	glFlush();

	auto tmp_result = particle_data_ref.m_buffer_swap;
	particle_data_ref.m_buffer_swap = particle_data_ref.m_buffer_position;
	particle_data_ref.m_buffer_position = tmp_result;

	return tmp_result;
}

GLint transform_feedback::ProccesVelocities(particle_data& particle_data_ref)
{
	// gives more misery and has no visible efect when disabled
	//glEnable(GL_RASTERIZER_DISCARD); 
	glUseProgram(m_program);

	// bind our buffer to the Transform feedback
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, particle_data_ref.m_buffer_swap);

	printOpenGLError();

	glm::mat4 ViewMatrix = getViewMatrix();
	//glUniform3f(m_uniform_point, 0, 0, 0);
	glm::vec4 pos = CursorToWorldspace(0.5); // pos(0, 0, 0, 0); // 
	glUniform3f(m_uniform_point, pos.x, pos.y, pos.z);

	glEnableVertexAttribArray(m_in_attrib_position);
	glBindBuffer(GL_ARRAY_BUFFER, particle_data_ref.m_buffer_position);
	glVertexAttribPointer(m_in_attrib_position, 4, GL_FLOAT, GL_FALSE, 0, nullptr);

	printOpenGLError();
	glEnableVertexAttribArray(m_in_attrib_velocity);
	glBindBuffer(GL_ARRAY_BUFFER, particle_data_ref.m_buffer_velocity);
	glVertexAttribPointer(m_in_attrib_velocity, 4, GL_FLOAT, GL_FALSE, 0, nullptr);

	printOpenGLError();
	// GL_POINTS
	// GL_LINES, GL_LINE_LOOP, GL_LINE_STRIP, GL_LINES_ADJACENCY, GL_LINE_STRIP_ADJACENCY
	// GL_TRIANGLES, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN, GL_TRIANGLES_ADJACENCY, GL_TRIANGLE_STRIP_ADJACENCY
	glBeginTransformFeedback(GL_POINTS);
	glDrawArrays(GL_POINTS, 0, particle_data::COUNT);
	glEndTransformFeedback();

	printOpenGLError();
	glDisableVertexAttribArray(m_in_attrib_position);
	glDisableVertexAttribArray(m_in_attrib_velocity);

	printOpenGLError();
	glFlush();

	auto tmp_result = particle_data_ref.m_buffer_swap;
	particle_data_ref.m_buffer_swap = particle_data_ref.m_buffer_velocity;
	particle_data_ref.m_buffer_velocity = tmp_result;

	return tmp_result;
}

void transform_feedback::clean()
{
	if (m_program != 0)
		glDeleteProgram(m_program);
	m_program = 0;
}
