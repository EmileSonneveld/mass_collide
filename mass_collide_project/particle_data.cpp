
#include <vector>
#include <algorithm>
#include <string>
#include <iostream>
#include <fstream>
#include <assert.h>
#include <random>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/norm.hpp>
using namespace glm;

#include <GL/glew.h>

#include "globals.h"
#include "particle_data.h"

void initialize_buffers(particle_data& particle_data_ref)
{

	// single serving data to init GPU stuff //
	///////////////////////////////////////////

	auto* data_random_pos = new vec4[particle_data::COUNT];
	auto* data_random_col = new GLubyte[particle_data::COUNT * 4];

	for (unsigned int i = 0; i < particle_data::COUNT; ++i){
		data_random_pos[i].x = (float)(rand() % 100) / 10.0f - 5.0f;
		data_random_pos[i].y = (float)(rand() % 100) / 10.0f - 5.0f;
		data_random_pos[i].z = (float)(rand() % 100) / 10.0f - 5.0f;
		data_random_pos[i].a = 0.04f; // size

		data_random_col[i + 0] = rand() % 128;
		data_random_col[i + 1] = rand() % 128;
		data_random_col[i + 2] = rand() % 128;
		data_random_col[i + 3] = 128;


		float size = 0.1f;
		data_random_pos[i].x = (i / 1 % 100) * size;
		data_random_pos[i].y = (i / 100 % 100) * size;
		data_random_pos[i].z = (i / 100 / 100 % 100) * size;
	}


	printOpenGLError();
	// Create and initialize the Buffer Objects on the GPU //
	//////////////////////////////////////////////////

	//glGenBuffers(1, &particle_data_ref.m_buffer_position);
	glBindBuffer(GL_ARRAY_BUFFER, particle_data_ref.m_buffer_position);
	glBufferData(
		GL_ARRAY_BUFFER,
		particle_data::COUNT * sizeof(vec4),
		data_random_pos,
		GL_DYNAMIC_DRAW
		);

	glBindBuffer(GL_ARRAY_BUFFER, particle_data_ref.m_buffer_position_swap);
	glBufferData(
		GL_ARRAY_BUFFER,
		particle_data::COUNT * sizeof(vec4), data_random_pos, GL_DYNAMIC_DRAW);


	//glGenBuffers(1, &particle_data_ref.m_buffer_color);
	glBindBuffer(GL_ARRAY_BUFFER, particle_data_ref.m_buffer_color);
	glBufferData(
		GL_ARRAY_BUFFER,
		particle_data::COUNT * 4 * sizeof(GLubyte),
		data_random_col,
		GL_DYNAMIC_DRAW
		);


	delete data_random_pos;
	delete data_random_col;

}

void initialize_velocity(particle_data& particle_data_ref)
{
	std::default_random_engine generator;
	std::normal_distribution<float> distribution(0, 0.1f);

	auto* data_random_vel = new vec4[particle_data::COUNT];
	for (unsigned int i = 0; i < particle_data::COUNT; ++i){
		vec4 tmp;
		tmp.x = distribution(generator);
		tmp.y = distribution(generator);
		tmp.z = distribution(generator);
		tmp.a = 20; // padding
		data_random_vel[i] = tmp;
	}

	vec4 tmp;
	tmp.x = 0.02f;
	tmp.y = 0.0f;
	tmp.z = 0.0f;
	tmp.a = 0; // padding
	data_random_vel[0] = tmp;

	//1.000000 1.414214 1.732051 2.000000 2.236068
	//for (unsigned int i = 0; i < nr_of_particles; ++i){
	//	std::cout << "x: " << data_random_vel[i].x << "\n";
	//}

	//glGenBuffers(1, &m_buffer_velocity);
	glBindBuffer(GL_ARRAY_BUFFER, particle_data_ref.m_buffer_velocity);
	glBufferData(
		GL_ARRAY_BUFFER,
		particle_data::COUNT * sizeof(vec4),
		data_random_vel,
		GL_STATIC_DRAW
		);

	delete data_random_vel;
}