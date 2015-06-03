
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

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "globals.h"
#include "particle_data.h"



void data::generate_colors_random()
{
	for (unsigned int i = 0; i < m_count; ++i){
		m_col[i].x = float(rand() % 256) / 256.0f;
		m_col[i].y = float(rand() % 256) / 256.0f;
		m_col[i].z = float(rand() % 256) / 256.0f;
		m_col[i].a = 1;
	}
}

void data::generate_colors_sinus()
{
	for(unsigned int i = 0; i < m_count; ++i){
		m_col[i].x = std::sin(m_pos[i].x*2.1f) / 2.f + 0.5f;
		m_col[i].y = std::sin(m_pos[i].y*2.2f) / 2.f + 0.5f;
		m_col[i].z = std::sin(m_pos[i].z*2.3f) / 2.f + 0.25f;
		m_col[i].a = 1;
	}
}

void data::generate_colors_gradient()
{
	for (unsigned int i = 0; i < m_count; ++i){
		m_col[i].x = float(i) / float(m_count);
		m_col[i].y = float(i) / float(m_count);
		m_col[i].z = float(i) / float(m_count);
		m_col[i].a = 1;
	}
}

void data::generate_positions_random()
{
	for (unsigned int i = 0; i < m_count; ++i){
		float size = GetPsSetting_Float("random_box_size", 10);
		m_pos[i].x = (0.5f - (float)(rand() % 100) * 0.01f) * size;
		m_pos[i].y = (0.5f - (float)(rand() % 100) * 0.01f) * size;
		m_pos[i].z = (0.5f - (float)(rand() % 100) * 0.01f) * size;
		m_pos[i].a = GetPsSetting_Float("particle_size", 0.04f);
	}
}

void data::generate_positions_structured()
{
	float particle_size = GetPsSetting_Float("particle_size", 0.04f);
	for (unsigned int i = 0; i < m_count; ++i){
		float size = 0.1f;
		m_pos[i].x = (i / 1 % 100) * size;
		m_pos[i].y = (i / 100 % 100) * size;
		m_pos[i].z = (i / 100 / 100 % 100) * size;
		m_pos[i].a = particle_size;
	}
}

void data::load_positions_from_model(const char* file_name)
{
	// auto assimpScene = aiImportFile(file_name, aiProcessPreset_TargetRealtime_MaxQuality);
	// auto nd = assimpScene->mRootNode;
	// 
	// for (unsigned int n = 0; n < nd->mNumMeshes; ++n) {
	// 	auto mesh = assimpScene->mMeshes[nd->mMeshes[n]];
	// 	mesh->mNumVertices;
	// 	mesh->mVertices;
	// }
	// 
	// aiReleaseImport(assimpScene);
}


void data::generate_velocities_random()
{
	std::default_random_engine generator;
	std::normal_distribution<float> distribution(0, GetPsSetting_Float("init_random_velocity", 0.05f));

	for (unsigned int i = 0; i < m_count; ++i){
		vec4 tmp;
		tmp.x = distribution(generator);
		tmp.y = distribution(generator);
		tmp.z = distribution(generator);
		tmp.a = 108; // padding
		m_vel[i] = tmp;
	}
}


void data::generate_indexes()
{
	m_indices.reserve(200000);
	float distance = GetPsSetting_Float("connection_max_distance", 0.35f);
	float distanseSqr = distance * distance;

	const int maxConn = 4;
	m_indices_alt.reserve(m_count * maxConn);
	m_length_alt.reserve(m_count * maxConn);

	for (unsigned int ia = 0; ia < m_count; ++ia)
	{
		int num = 0;
		for (unsigned int ib = 0; ib < m_count; ++ib){
			if (ia == ib) continue;
			auto len2 = glm::length2(m_pos[ia] - m_pos[ib]);
			if (len2 < distanseSqr){

				m_indices_alt.push_back(ib);
				m_length_alt.push_back(sqrt(len2));


				m_indices.push_back(ia);
				m_indices.push_back(ib);

				++num;
				if (num >= maxConn){
					break;
				}
			}
		}
		for (int rest = num; rest < maxConn; ++rest)
		{
			m_indices_alt.push_back(108); // m_indices_alt.size()); // a no connection magic number
			m_length_alt.push_back(108); // must not be used
		}
	}

	m_connection_count = m_indices.size();

	std::cout << "m_indices_alt [";
	for (unsigned int i = 0; i < m_indices_alt.size(); i += 1){
		if (i%maxConn == 0){
			std::cout << "\n   ";
			if (i > 5){
				std::cout << "...";
				break;
			}
		}
		std::cout << m_indices_alt[i] << " ";
	}
	std::cout << "\n]\n";
}



void data::initialize_buffers_from_data(particle_data& particle_data_ref)
{
	particle_data_ref.COUNT = m_count;
	particle_data_ref.CONNECTION_COUNT = m_connection_count;

	glBindBuffer(GL_TEXTURE_BUFFER, particle_data_ref.buffer[position]);
	glBufferData(GL_TEXTURE_BUFFER, particle_data_ref.COUNT * sizeof(vec4), m_pos, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, particle_data_ref.buffer[color]);
	glBufferData(GL_ARRAY_BUFFER, particle_data_ref.COUNT * sizeof(vec4), m_col, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, particle_data_ref.buffer[velocity]);
	glBufferData(GL_ARRAY_BUFFER, particle_data_ref.COUNT * sizeof(vec4), m_vel, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, particle_data_ref.buffer[swap]);
	glBufferData(GL_ARRAY_BUFFER, particle_data_ref.COUNT * sizeof(vec4), nullptr, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, particle_data_ref.buffer[connection_index]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), &m_indices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_TEXTURE_BUFFER, particle_data_ref.buffer[connection_index_alt]);
	glBufferData(GL_TEXTURE_BUFFER, m_indices_alt.size() * sizeof(unsigned int), &m_indices_alt[0], GL_DYNAMIC_DRAW);

	glBindBuffer(GL_TEXTURE_BUFFER, particle_data_ref.buffer[connection_length_alt]);
	glBufferData(GL_TEXTURE_BUFFER, m_length_alt.size() * sizeof(float), &m_length_alt[0], GL_DYNAMIC_DRAW);

}


void data::doAllTheInitisation(particle_data& particle_data_ref)
{
	m_count = GetPsSetting_Int("count", 100);
	m_pos = new vec4[m_count];
	m_col = new vec4[m_count];
	m_vel = new vec4[m_count];
	if (GetPsSetting_Bool("use_structured_spawning", true))
		this->generate_positions_structured();
	else
		this->generate_positions_random();

	generate_colors_random();
	generate_velocities_random();
	generate_indexes();

	initialize_buffers_from_data(particle_data_ref);
}
