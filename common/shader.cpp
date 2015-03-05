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
//#include "globals.h"

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
		printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", file_path);
	}
	return shaderCode;
}


GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path){

	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	auto shader_vertex = getCodeFromFile(vertex_file_path);
	auto shader_fragment = getCodeFromFile(fragment_file_path);


	GLint Result = GL_FALSE;
	int InfoLogLength;



	// Compile Vertex Shader
	printf("Compiling shader : %s\n", vertex_file_path);
	char const * VertexSourcePointer = shader_vertex.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> VertexShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}



	// Compile Fragment Shader
	printf("Compiling shader : %s\n", fragment_file_path);
	char const * FragmentSourcePointer = shader_fragment.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}



	// Link the program
	printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> ProgramErrorMessage(InfoLogLength+1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}

/*
// adition by Emile
GLuint LoadComputeShader(const char * compute_file_path){


	printOpenGLError();

	// Read the Vertex Shader code from the file
	std::string ShaderStream;
	std::ifstream VertexShaderStream(compute_file_path, std::ios::in);
	if (VertexShaderStream.is_open()){
		std::string Line = "";
		while (getline(VertexShaderStream, Line))
			ShaderStream += "\n" + Line;
		VertexShaderStream.close();
	}
	else{
		printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", compute_file_path);
		//getchar();
		return 0;
	}
	printOpenGLError();


	const char* FuckingCString = ShaderStream.c_str();
	// Create th compute program, to which the compute shader will be assigned
	GLuint gComputeProgram = glCreateProgram();
	// Create and compile the compute shader
	printOpenGLError();
	GLuint mComputeShader = glCreateShader(GL_COMPUTE_SHADER); // invalid operation needs openGl 4
	printOpenGLError();
	glShaderSource(mComputeShader, 1, &FuckingCString, 0);
	printOpenGLError();
	glCompileShader(mComputeShader);



	printOpenGLError();

	const int log_max = 1000;
	GLsizei get_len; //futile
	char get_log[log_max];
	// Check if there were any issues when compiling the shader
	int rvalue;
	glGetShaderiv(mComputeShader, GL_COMPILE_STATUS, &rvalue);
	if (!rvalue)
	{
		glGetShaderInfoLog(mComputeShader, log_max, &get_len, get_log);
		printf("Error: Compiler log:\n%s\n", get_log);
		return NULL;
	}

	// Attach and link the shader against to the compute program
	glAttachShader(gComputeProgram, mComputeShader);
	glLinkProgram(gComputeProgram);


	printOpenGLError();

	// Check if there were some issues when linking the shader.
	glGetProgramiv(gComputeProgram, GL_LINK_STATUS, &rvalue);
	if (!rvalue)
	{
		//Link info
		//---------
		//No shader objects attached.
		glGetProgramInfoLog(gComputeProgram, log_max, &get_len, get_log);
		printf("Error: Linker log:\n%s\n", get_log);
		return NULL;
	}

	printOpenGLError();

	return gComputeProgram;
}
*/