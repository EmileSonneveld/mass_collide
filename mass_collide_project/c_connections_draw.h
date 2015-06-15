#pragma once

#include <string>

#include <GL/glew.h>

#include "calculate.h"

struct particle_data;

class c_connections_draw : calculate
{
public:
	c_connections_draw() :
		m_program(0),
		m_uniform_matrix(0)
	{}
	virtual ~c_connections_draw(){}

	void initialize_buffers(particle_data& particle_data_ref);
	void initialize();
	void process(particle_data& particle_data_ref);
	void clean();

private:
	GLuint m_program;
	GLint m_in_attrib_position = 0;
	GLint m_in_attrib_color = 1;

	GLint m_uniform_matrix;
};
