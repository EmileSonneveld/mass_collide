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

void connections::initialize(std::string file_name)
{
	clean();
	m_program = CompileComputeShader(file_name.c_str());
	m_uniform_point = glGetUniformLocation(m_program, "point");
	m_in_attrib_position = glGetAttribLocation(m_program, "inPosition");
	m_in_attrib_velocity = glGetAttribLocation(m_program, "inVelocity");
}

void Draw()
{
    // draw with simple shader
}

void connections::clean()
{
	if (m_program != 0)
		glDeleteProgram(m_program);
	m_program = 0;
}
