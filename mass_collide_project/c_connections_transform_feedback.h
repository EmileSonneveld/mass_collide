#pragma once

#include <string>

#include <GL/glew.h>

#include "calculate.h"

struct particle_data;

class c_connections_transform_feedback : calculate
{
public:
	c_connections_transform_feedback() :
		m_program(0)
	{}
	virtual ~c_connections_transform_feedback(){}

	void initialize(std::string file_name, bufferName output_buffer_name);
	void process(particle_data& particle_data_ref);
	void clean();

private:
	GLuint m_program;
	GLint m_in_attrib_position = 0; //defined in shader
	GLint m_in_attrib_velocity = 1; //defined in shader

	GLuint m_uniform_samplerPosition;
	GLuint m_uniform_samplerVelocity;

	GLuint m_texture_buffer; // A Texture wich contains a link to a Buffer

	bufferName m_transform_feedback_out = bufferName::position;
};

