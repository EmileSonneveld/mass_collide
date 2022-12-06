#pragma once

#include <string>

#include <GL/glew.h>

#include "calculate.h"

struct particle_data;

class c_transform_feedback : private calculate
{
public:
	void initialize(const std::string& file_name, const bufferName output_buffer_name, const particle_data& particle_data_ref);
	void process(particle_data& particle_data_ref) override;
	void clean() override;

private:
	GLuint m_program = 0;
	GLint m_in_attrib_position = 0; //defined in shader
	GLint m_in_attrib_velocity = 1; //defined in shader

	bufferName m_transform_feedback_out = bufferName::position;
	GLint m_uniform_point = 0;
};
