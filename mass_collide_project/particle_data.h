#pragma once

struct particle_data
{
	particle_data() :
		m_buffer_position(0),
		m_buffer_swap(0),
		m_buffer_color(0),
		m_buffer_velocity(0)
	{
		glGenBuffers(1, &m_buffer_position);
		glGenBuffers(1, &m_buffer_swap);
		glGenBuffers(1, &m_buffer_color);
		glGenBuffers(1, &m_buffer_velocity);
	}
	void clean()
	{
		glDeleteBuffers(1, &m_buffer_position);
		glDeleteBuffers(1, &m_buffer_swap);
		glDeleteBuffers(1, &m_buffer_color);
		glDeleteBuffers(1, &m_buffer_velocity);
	}

	GLuint m_buffer_position;
	GLuint m_buffer_swap;
	GLuint m_buffer_color;
	GLuint m_buffer_velocity;

	unsigned int COUNT = 100000;
};

void initialize_buffers(particle_data& particle_data_ref);
void initialize_velocity(particle_data& particle_data_ref);
