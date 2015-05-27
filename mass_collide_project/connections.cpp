#include "connections.h"

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


static GLint CompileComputeShader(const GLchar* filename)
{
	printOpenGLError();
	GLuint shader = LoadTemporaryShader(filename, GL_VERTEX_SHADER);

	GLuint program = glCreateProgram();
	glAttachShader(program, shader);

	glDeleteShader(shader);

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

void connections::initialize()
{
	clean();
	m_program = LoadShaders("rc/simple_vert.glsl", "rc/simple_frag.glsl");
	printOpenGLError();
	m_uniform_matrix = glGetUniformLocation(m_program, "ViewProjectionMatrix");
	m_in_attrib_position = glGetAttribLocation(m_program, "inPosition");
	m_in_attrib_color = glGetAttribLocation(m_program, "inColor");
}

void connections::draw(particle_data& particle_data_ref)
{
	glUseProgram(m_program);
	printOpenGLError();

	glCullFace(GL_FRONT_AND_BACK);

	glm::mat4 ProjectionMatrix = getProjectionMatrix();
	glm::mat4 ViewMatrix = getViewMatrix();
	glm::mat4 ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
	//glm::mat4 ViewProjectionMatrix = getProjectionMatrix() * getViewMatrix();
	glUniformMatrix4fv(m_uniform_matrix, 1, GL_FALSE, &ViewProjectionMatrix[0][0]);

	glEnableVertexAttribArray(m_in_attrib_position);
	glBindBuffer(GL_ARRAY_BUFFER, particle_data_ref.m_buffer_position);
	glVertexAttribPointer(m_in_attrib_position, 4, GL_FLOAT, GL_FALSE, 0, nullptr);

	glEnableVertexAttribArray(m_in_attrib_color);
	glBindBuffer(GL_ARRAY_BUFFER, particle_data_ref.m_buffer_color);
	glVertexAttribPointer(m_in_attrib_color, 4, GL_FLOAT, GL_FALSE, 0, nullptr);


	//glBindBuffer(GL_ARRAY_BUFFER, particle_data_ref.m_buffer_position);
	printOpenGLError();
	// Index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, particle_data_ref.m_buffer_connection_index);
	printOpenGLError();

	glDrawElements(
		GL_LINES,
		particle_data_ref.CONNECTION_COUNT,
		GL_UNSIGNED_INT,
		(void*)0
		);
	printOpenGLError();

}

void connections::clean()
{
	if (m_program != 0)
		glDeleteProgram(m_program);
	m_program = 0;
}
