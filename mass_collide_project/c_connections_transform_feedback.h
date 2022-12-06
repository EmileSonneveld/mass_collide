#pragma once

#include <string>

#include <GL/glew.h>

#include "calculate.h"

struct particle_data;

class c_connections_transform_feedback : private calculate
{
public:
	void initialize(const std::string& file_name, const bufferName output_buffer_name, const particle_data& particle_data_ref);
	void process(particle_data& particle_data_ref) override;
	void clean() override;

private:
	GLuint m_program = 0;
	GLint m_in_attrib_position = -2;
	GLint m_in_attrib_velocity = -2;

	GLint m_uniform_samplerPosition = -2;
	GLint m_uniform_samplerOtherIndex = -2;
	GLint m_uniform_samplerLengthToOther;

	GLuint m_texture_buffer = -2; // A Texture wich contains a link to a Buffer
	GLuint m_texture_buffer_OtherIndex = -2;
	GLuint m_texture_buffer_LengthToOther;

	bufferName m_transform_feedback_out;
};

