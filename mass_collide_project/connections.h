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
		m_uniform_matrix(0)
	{}
	virtual ~connections(){}

	void initialize_buffers(particle_data& particle_data_ref);
	void initialize();
	GLint Procces(particle_data& particle_data_ref);
	void draw(particle_data& particle_data_ref);
	void clean();

private:
	GLint m_program;
	GLint m_in_attrib_position = 0;

	GLint m_uniform_matrix;
};


//GLint CompileComputeShader(const GLchar* filename);
