#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
using namespace std;

#include <stdlib.h>
#include <string.h>

#include <GL/glew.h>

#include "shader.hpp"

std::string getCodeFromFile(const char * file_path)
{
	// Read the Vertex Shader code from the file
	std::string shaderCode;
	std::ifstream fileStream(file_path, std::ios::in);
	if (fileStream.is_open()){
		std::string Line = "";
		while (getline(fileStream, Line))
			shaderCode += "\n" + Line;
		fileStream.close();
	}
	else{
		printf("Impossible to open %s. Are you in the right directory?\n", file_path);
	}
	return shaderCode;
}

GLuint LoadTemporaryShader(const char * glsl_file_path, GLenum kind)
{
	GLuint shaderID = glCreateShader(kind);

	auto shader_text = getCodeFromFile(glsl_file_path);

	GLint Result = GL_FALSE;
	int InfoLogLength;

	// Compile Vertex Shader
	printf("Compiling shader : %s\n", glsl_file_path);
	char const * VertexSourcePointer = shader_text.c_str();
	glShaderSource(shaderID, 1, &VertexSourcePointer, NULL);
	glCompileShader(shaderID);

	// Check Vertex Shader
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0){
		std::vector<char> ErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(shaderID, InfoLogLength, NULL, &ErrorMessage[0]);
		printf("%s\n", &ErrorMessage[0]);
	}

	return shaderID; // !needs to get deleted after program creation!
}

GLuint LoadShaders(const char * vertex_file_path, const char * fragment_file_path){

	// Create the shaders
	GLuint VertexShaderID = LoadTemporaryShader(vertex_file_path, GL_VERTEX_SHADER);
	GLuint FragmentShaderID = LoadTemporaryShader(fragment_file_path, GL_FRAGMENT_SHADER);

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
	if ( InfoLogLength > 0 ){
		std::vector<char> ErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ErrorMessage[0]);
		printf("%s\n", &ErrorMessage[0]);
	}

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}
