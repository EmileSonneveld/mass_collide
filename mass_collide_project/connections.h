#pragma once

#include <string>

#include <GL/glew.h>

#include "calculate.h"

struct particle_data;

class connections : calculate
{
public:
	connections() :
		m_program(0),
		m_uniform_point()
	{}
	virtual ~connections(){}

	void initialize(std::string file_name = "rc/compute.glsl");
	GLint Procces(particle_data& particle_data_ref);
    void draw();
	void clean();

private:
	GLint m_program;
	GLint m_in_attrib_position = 0; //defined in shader
	GLint m_in_attrib_velocity = 1; //defined in shader

	GLint m_uniform_point;
};


//GLint CompileComputeShader(const GLchar* filename);
