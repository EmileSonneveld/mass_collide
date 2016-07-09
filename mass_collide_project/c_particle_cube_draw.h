#pragma once

#include <GL/glew.h>

#include "calculate.h"

struct particle_data;

class c_particle_cube_draw : calculate
{
public:
	c_particle_cube_draw() :
		m_buffer_billboard_vertex(0),
        m_program(0),
        m_in_attrib_square(0),
        m_in_attrib_position(1),
        m_in_attrib_color(2),
		m_uniform_ViewProjMatrix(0)
	{}
	virtual ~c_particle_cube_draw(){}

	void initialize();
	void process(particle_data& particle_data_ref);
	void clean();

private:
	GLuint m_buffer_billboard_vertex;

	GLuint m_program;
	GLint m_in_attrib_square = 0; //defined in shader
	GLint m_in_attrib_position = 1; //defined in shader
	GLint m_in_attrib_color = 2; //defined in shader

	GLuint m_uniform_ViewProjMatrix;
};

