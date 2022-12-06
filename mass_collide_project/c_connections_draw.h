#pragma once

#include <string>

#include <GL/glew.h>

#include "calculate.h"

struct particle_data;

class c_connections_draw : private calculate
{
public:
	void initialize(const particle_data& particle_data_ref);
	void process(particle_data& particle_data_ref) override;
	void clean() override;

private:
	GLuint m_program = 0;
	GLint m_in_attrib_position = 0;
	GLint m_in_attrib_color = 1;

	GLint m_uniform_matrix = 0;
};
