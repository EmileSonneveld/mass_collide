#pragma once

#include <string>

#include <GL/glew.h>

#include "calculate.h"

struct particle_data;

class c_connections_transform_feedback : calculate
{
public:
	c_connections_transform_feedback() :
		m_program(-2),
		m_in_attrib_position(-2),
		m_in_attrib_velocity(-2),
		m_uniform_samplerPosition(-2),
		m_uniform_samplerOtherIndex(-2),
		m_texture_buffer(-2),
		m_texture_buffer_OtherIndex(-2)
	{}
	virtual ~c_connections_transform_feedback(){}

	void initialize(std::string file_name, bufferName output_buffer_name);
	void process(particle_data& particle_data_ref);
	void clean();

private:
	GLuint m_program;
	GLint m_in_attrib_position;
	GLint m_in_attrib_velocity;

	GLint m_uniform_samplerPosition;
	GLint m_uniform_samplerOtherIndex;

	GLuint m_texture_buffer; // A Texture wich contains a link to a Buffer
	GLuint m_texture_buffer_OtherIndex;

	bufferName m_transform_feedback_out;
};

