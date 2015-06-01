#pragma once


struct data_struct 
{
	GLuint buffer;
	void* cpuPtr;
	bool isBufferUpToDate;
	unsigned int size;
};

// add an element to this enum to use a shared
enum
{
	swap,
	position,
	color,
	velocity,
	connection_index,
	size_element
};

struct particle_data
{
	particle_data()
	{
		glGenBuffers(size_element , &buffer[0]);
	}
	void clean()
	{
		glDeleteBuffers(size_element , &buffer[0]);
	}

	GLuint buffer[size_element ];

	unsigned int CONNECTION_COUNT;
	unsigned int COUNT = 100000;
};

void initialize_buffers(particle_data& particle_data_ref);
void initialize_swap_buffer(particle_data& particle_data_ref);
void initialize_velocity(particle_data& particle_data_ref);
