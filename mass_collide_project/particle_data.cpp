
#include <vector>
#include <algorithm>
#include <string>
#include <iostream>
#include <fstream>
#include <assert.h>
#include <random>
#include <math.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/norm.hpp>
using namespace glm;

#include <GL/glew.h>

#include "globals.h"
#include "particle_data.h"


static void pattern_fill_cube_nicely(unsigned int count, vec4* data_pos, vec4* data_col)
{
	for (unsigned int i = 0; i < count; ++i){
		float size = 0.1f;
		data_pos[i].x = (i / 1 % 100) * size;
		data_pos[i].y = (i / 100 % 100) * size;
		data_pos[i].z = (i / 100 / 100 % 100) * size;
		data_pos[i].a = GetPsSetting_Float("particle_size", 0.04f);

		data_col[i].x = std::sin(data_pos[i].x*2.1f) / 2.f + 0.5f;
		data_col[i].y = std::sin(data_pos[i].y*2.2f) / 2.f + 0.5f;
		data_col[i].z = std::sin(data_pos[i].z*2.3f) / 2.f + 0.25f;
		data_col[i].a = 1;

		data_col[i].x *= (float)i / count;
		data_col[i].y *= (float)i / count;
		data_col[i].z *= (float)i / count;
	}

}

static void pattern_fill_cube_random(unsigned int count, vec4* data_pos, vec4* data_col)
{
	for (unsigned int i = 0; i < count; ++i){
		float size = GetPsSetting_Float("random_box_size", 10);
		data_pos[i].x = (0.5f - (float)(rand() % 100) * 0.01f) * size;
		data_pos[i].y = (0.5f - (float)(rand() % 100) * 0.01f) * size;
		data_pos[i].z = (0.5f - (float)(rand() % 100) * 0.01f) * size;
		data_pos[i].a = GetPsSetting_Float("particle_size", 0.04f);

		data_col[i].x = float(rand() % 256) / 256.0f;
		data_col[i].y = float(rand() % 256) / 256.0f;
		data_col[i].z = float(rand() % 256) / 256.0f;
		data_col[i].a = 1;
	}

}

void initialize_buffers(particle_data& particle_data_ref)
{
	// single serving data to init GPU stuff //
	///////////////////////////////////////////

	auto* data_pos = new vec4[particle_data_ref.COUNT];
	auto* data_col = new vec4[particle_data_ref.COUNT];

	if (GetPsSetting_Bool("use_structured_spawning", true))
		pattern_fill_cube_nicely(particle_data_ref.COUNT, data_pos, data_col);
	else
		pattern_fill_cube_random(particle_data_ref.COUNT, data_pos, data_col);
	


	std::vector<unsigned int>data_indices(190000);

	float distance = GetPsSetting_Float("connection_max_distance", 0.35f);
	float distanseSqr = distance * distance;

	for (unsigned int ia = 0; ia < particle_data_ref.COUNT; ++ia){
		for (unsigned int ib = 0; ib < particle_data_ref.COUNT; ++ib){
			if (ia == ib) continue;
			auto len2 = glm::length2(data_pos[ia] - data_pos[ib]);
			if (len2 < distanseSqr){
				data_indices.push_back(ia);
				data_indices.push_back(ib);
			}
		}
	}
	particle_data_ref.CONNECTION_COUNT = data_indices.size();

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, particle_data_ref.m_buffer_connection_index);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, data_indices.size() * sizeof(unsigned int), &data_indices[0], GL_STATIC_DRAW);



	// Create and initialize the Buffer Objects on the GPU //
	/////////////////////////////////////////////////////////

	glBindBuffer(GL_ARRAY_BUFFER, particle_data_ref.m_buffer_position);
	glBufferData(
		GL_ARRAY_BUFFER,
		particle_data_ref.COUNT * sizeof(vec4),
		data_pos,
		GL_DYNAMIC_DRAW
		);

	glBindBuffer(GL_ARRAY_BUFFER, particle_data_ref.m_buffer_color);
	glBufferData(
		GL_ARRAY_BUFFER,
		particle_data_ref.COUNT * sizeof(vec4), // GLubyte
		data_col,
		GL_DYNAMIC_DRAW
		);

	delete data_pos;
	delete data_col;
}

void initialize_swap_buffer(particle_data& particle_data_ref)
{
	glBindBuffer(GL_ARRAY_BUFFER, particle_data_ref.m_buffer_swap);
	glBufferData(
		GL_ARRAY_BUFFER,
		particle_data_ref.COUNT * sizeof(vec4), nullptr,
		GL_DYNAMIC_DRAW
		);
}

void initialize_velocity(particle_data& particle_data_ref)
{
	std::default_random_engine generator;
	std::normal_distribution<float> distribution(0, 0.05f);

	auto* data_random_vel = new vec4[particle_data_ref.COUNT];
	for (unsigned int i = 0; i < particle_data_ref.COUNT; ++i){
		vec4 tmp;
		tmp.x = distribution(generator);
		tmp.y = distribution(generator);
		tmp.z = distribution(generator);
		tmp.a = 108; // padding
		data_random_vel[i] = tmp;
	}

	glBindBuffer(GL_ARRAY_BUFFER, particle_data_ref.m_buffer_velocity);
	glBufferData(
		GL_ARRAY_BUFFER,
		particle_data_ref.COUNT * sizeof(vec4),
		data_random_vel,
		GL_STATIC_DRAW
		);

	delete data_random_vel;
}
