#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
using namespace std;

#include <stdlib.h>
#include <string.h>
#include <sstream>

#include <GL/glew.h>

#include "shader.hpp"
#include "../mass_collide_project/Util.h"


std::string getCodeFromFile(const char * file_path)
{
	// Read the Vertex Shader code from the file
	std::string shaderCode;
	std::ifstream fileStream(file_path, std::ios::in);
	if (fileStream.is_open()) {
		std::string Line = "";
		while (getline(fileStream, Line))
			shaderCode += "\n" + Line;
		fileStream.close();
	}
	else {
		printf("Impossible to open %s. Are you in the right directory?\n", file_path);
	}
	return shaderCode;
}

GLuint LoadTemporaryShader(const string& shader_text, const GLenum kind)
{
	GLuint shaderID = glCreateShader(kind);

	//auto shader_text = getCodeFromFile(glsl_file_path);

	GLint Result = GL_FALSE;
	int InfoLogLength;

	// Compile Vertex Shader
	//printf("Compiling shader : %s\n", glsl_file_path);
	char const * VertexSourcePointer = shader_text.c_str();
	glShaderSource(shaderID, 1, &VertexSourcePointer, NULL);
	glCompileShader(shaderID);

	// Check Vertex Shader
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> ErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(shaderID, InfoLogLength, NULL, &ErrorMessage[0]);
		if (ErrorMessage[0] != '\0')
			printf("%s\n", &ErrorMessage[0]);
	}

	return shaderID; // !needs to get deleted after program creation!
}

GLuint LoadShaderFromPath(const char * vertex_file_path, const char * fragment_file_path) {
	auto shader_vert_text = getCodeFromFile(vertex_file_path);
	auto shader_frag_text = getCodeFromFile(fragment_file_path);
	return LoadShaderFromString(shader_vert_text, shader_frag_text);
}

GLuint LoadShaderFromString(const string& vertex_string, const string& fragment_string) {

	// Create the shaders
	GLuint VertexShaderID = LoadTemporaryShader(vertex_string, GL_VERTEX_SHADER);
	GLuint FragmentShaderID = LoadTemporaryShader(fragment_string, GL_FRAGMENT_SHADER);

	// Link the program
	printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	GLint Result = GL_FALSE;
	int InfoLogLength;
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> ErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ErrorMessage[0]);
		if (ErrorMessage[0] != '\0')
			printf("%s\n", &ErrorMessage[0]);
	}

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}

GLuint LoadShaderWithTransformFeedbackFromPath(const char* filename)
{
	auto shader_text = getCodeFromFile(filename);
	shader_text = Util::ReplaceTokensWith_particleData(shader_text);
	return LoadShaderWithTransformFeedbackFromString(shader_text);
}

GLuint LoadShaderWithTransformFeedbackFromString(const std::string& shader_text)
{
	GLuint shader = LoadTemporaryShader(shader_text, GL_VERTEX_SHADER);

	GLuint program = glCreateProgram();
	glAttachShader(program, shader);

	// transform feedback specefic
	const GLchar* feedbackVaryings[] = { "outValue" };
	// or GL_SEPARATE_ATTRIBS GL_INTERLEAVED_ATTRIBS
	glTransformFeedbackVaryings(program, 1, feedbackVaryings, GL_INTERLEAVED_ATTRIBS);

	glLinkProgram(program);
	glValidateProgram(program);

	// Check the program
	{
		GLint Result = GL_FALSE;
		int InfoLogLength;
		glGetProgramiv(program, GL_LINK_STATUS, &Result);
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &InfoLogLength);
		if (InfoLogLength > 0) {
			std::vector<char> ErrorMessage(InfoLogLength + 1);
			glGetProgramInfoLog(program, InfoLogLength, NULL, &ErrorMessage[0]);
			if (ErrorMessage[0] != '\0')
				printf("%s\n", &ErrorMessage[0]);
		}
	}
	glUseProgram(program);

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	return program;
}