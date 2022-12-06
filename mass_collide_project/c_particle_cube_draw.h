#pragma once

#include <GL/glew.h>

#include "calculate.h"

struct particle_data;

class c_particle_cube_draw : private calculate
{
public:
	void initialize();
	void process(particle_data& particle_data_ref) override;
	void clean() override;

private:
	GLuint m_buffer_billboard_vertex = 0;

	GLuint m_program = 0;
	GLint m_in_attrib_square = 0; //defined in shader
	GLint m_in_attrib_position = 1; //defined in shader
	GLint m_in_attrib_color = 2; //defined in shader

	GLuint m_uniform_ViewProjMatrix = 0;
};
