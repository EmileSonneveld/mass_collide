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
#include "csv.h"



void particle_data_init::generate_colors_random()
{
	if (m_col == nullptr)
		m_col = new vec4[m_count];
	for (unsigned int i = 0; i < m_count; ++i){
		m_col[i].x = float(rand() % 256) / 256.0f;
		m_col[i].y = float(rand() % 256) / 256.0f;
		m_col[i].z = float(rand() % 256) / 256.0f;
		m_col[i].a = 1;
	}
}

void particle_data_init::generate_colors_sinus()
{
	if (m_col == nullptr)
		m_col = new vec4[m_count];
	for (unsigned int i = 0; i < m_count; ++i){
		m_col[i].x = std::sin(m_pos[i].x*2.1f) / 2.f + 0.5f;
		m_col[i].y = std::sin(m_pos[i].y*2.2f) / 2.f + 0.5f;
		m_col[i].z = std::sin(m_pos[i].z*2.3f) / 2.f + 0.25f;
		m_col[i].a = 1;
	}
}

void particle_data_init::generate_colors_gradient()
{
	if (m_col == nullptr)
		m_col = new vec4[m_count];
	for (unsigned int i = 0; i < m_count; ++i){
		m_col[i].x = float(i) / float(m_count);
		m_col[i].y = float(i) / float(m_count);
		m_col[i].z = float(i) / float(m_count);
		m_col[i].a = 1;
	}
}

void particle_data_init::generate_positions_random()
{
	if (m_pos == nullptr)
		m_pos = new vec4[m_count];
	for (unsigned int i = 0; i < m_count; ++i){
		float size = GetPsSetting_Float("random_box_size", 10);
		m_pos[i].x = (0.5f - (float)(rand() % 100) * 0.01f) * size;
		m_pos[i].y = (0.5f - (float)(rand() % 100) * 0.01f) * size;
		m_pos[i].z = (0.5f - (float)(rand() % 100) * 0.01f) * size;
		m_pos[i].a = GetPsSetting_Float("particle_size", 0.04f);
	}
}

void particle_data_init::generate_positions_structured()
{
	if (m_pos == nullptr)
		m_pos = new vec4[m_count];
	float particle_size = GetPsSetting_Float("particle_size", 0.04f);
	unsigned int sizecount = (unsigned int)GetPsSetting_Int("structured_box_sizecount", 50);
	float size = GetPsSetting_Float("structured_box_distance", 0.2f);
	for (unsigned int i = 0; i < m_count; ++i){
		m_pos[i].x = (i / 1 % sizecount) * size;
		m_pos[i].z = (i / sizecount % sizecount) * size;
		m_pos[i].y = (i / sizecount / sizecount) * size;
		m_pos[i].a = particle_size;
	}
}

void particle_data_init::load_positions_from_csv()
{
	float particle_size = GetPsSetting_Float("particle_size", 0.04f);
	const float scale = 20;
	std::vector<vec4> pos;
	pos.reserve(100000);
	/*{
		auto file_name = "C:/Users/emilesonneveld/Documents/GitHub/2017_04_28_MetroMaps/dataFromOpenData/shapes.txt";

		//shape_id,shape_pt_lat,shape_pt_lon,shape_pt_sequence
		//001m0006, 50.839382, 4.398154, 10001
		io::CSVReader<2> in(file_name);
		in.read_header(io::ignore_extra_column, "shape_pt_lat", "shape_pt_lon");
		float shape_pt_lat; float shape_pt_lon;
		while (in.read_row(shape_pt_lat, shape_pt_lon)) {
			pos.push_back(vec4((shape_pt_lat - 50)*scale, 0, (shape_pt_lon - 3)*scale, particle_size));
		}
	}

	{
		auto file_name = "C:/Users/emilesonneveld/Documents/GitHub/2017_04_28_MetroMaps/dataFromOpenData/routes.txt";
		
		//route_id, route_short_name, route_long_name, route_desc, route_type, route_url, route_color, route_text_color
		//001m - "GARE DE L'OUEST - STOCKEL", 1, "GARE DE L'OUEST - STOCKEL", , 1, , C4008F, FFFFFF
		io::CSVReader<2> in(file_name);
		in.read_header(io::ignore_extra_column, "shape_pt_lat", "shape_pt_lon");
		float shape_pt_lat; float shape_pt_lon;
		while (in.read_row(shape_pt_lat, shape_pt_lon)) {
			pos.push_back(vec4((shape_pt_lat - 50)*scale, 0, (shape_pt_lon - 3)*scale, particle_size));
		}
	}*/

	{
		auto file_name = "C:/Users/emilesonneveld/Documents/GitHub/2017_04_28_MetroMaps/dataFromOpenData/stops.txt";
		//stop_id, stop_code, stop_name, stop_desc, stop_lat, stop_lon, zone_id, stop_url, location_type
		//0089, , "MONTGOMERY", , 50.838120, 4.409113, , , 0
		io::CSVReader<2> in(file_name);
		in.read_header(io::ignore_extra_column, "stop_lat", "stop_lon");
		float stop_lat; float stop_lon;
		while (in.read_row(stop_lat, stop_lon)) {
			pos.push_back(vec4((stop_lat - 50)*scale, 0, (stop_lon - 3)*scale, particle_size));
		}
	}

	{
		auto file_name = "C:/Users/emilesonneveld/Documents/GitHub/2017_04_28_MetroMaps/dataFromOpenData/stop_times.txt";
		//trip_id, arrival_time, departure_time, stop_id, stop_sequence, pickup_type, drop_off_type
		//95466486 - B20170306 - BRESIM01 - Semaine - 03, 20:48 : 00, 20 : 48 : 00, 1142, 1, 0, 0
		io::CSVReader<2> in(file_name);
		in.read_header(io::ignore_extra_column, "trip_id", "stop_lon");
		float stop_lat; float stop_lon;
		while (in.read_row(stop_lat, stop_lon)) {
			pos.push_back(vec4((stop_lat - 50)*scale, 0, (stop_lon - 3)*scale, particle_size));
		}
	}

	m_count = pos.size();
	if (m_pos == nullptr)
		m_pos = new vec4[m_count];
	std::copy(pos.begin(), pos.end(), m_pos);
}

void particle_data_init::load_positions_from_model(const char* file_name)
{
	auto assimpScene = aiImportFile(file_name, aiProcess_JoinIdenticalVertices);
	//auto nd = assimpScene->mRootNode;  aiProcessPreset_TargetRealtime_MaxQuality |
	float particle_size = GetPsSetting_Float("particle_size", 0.04f);

	std::vector<aiVector3D> meshCopy;
	for (unsigned int n = 0; n < assimpScene->mNumMeshes; ++n) {
		auto mesh = assimpScene->mMeshes[n];

		m_count = mesh->mNumVertices;
		m_pos = new vec4[m_count];
		meshCopy.reserve(mesh->mNumVertices);
		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			auto vert = mesh->mVertices[i];
			m_pos[i] = vec4(vert.x, vert.y, vert.z, particle_size);
			meshCopy.push_back(vert);
		}
		break; // only supports first mesh
	}

	aiReleaseImport(assimpScene);
}


void particle_data_init::generate_velocities_random()
{
	if (m_vel == nullptr)
		m_vel = new vec4[m_count];
	auto rand = GetPsSetting_Float("init_random_velocity", 0.0f);
	if (rand == 0)
		return;

	std::default_random_engine generator;
	std::normal_distribution<float> distribution(0, rand);

	for (unsigned int i = 0; i < m_count; ++i){
		vec4 tmp;
		tmp.x = distribution(generator);
		tmp.y = distribution(generator);
		tmp.z = distribution(generator);
		tmp.a = 108; // padding
		m_vel[i] = tmp;
	}
}

// class generator:
struct c_unique {
	int current;
	c_unique() { current = 0; }
	int operator()() { return current++; }
} UniqueNumber;


// Following line to avoid error:
// particle_data.cpp:(.text+0x1303): undefined reference to `particle_data_init::EMPTY_VALUE'
const unsigned int particle_data_init::EMPTY_VALUE;


void particle_data_init::generate_indexes()
{
	const unsigned int NOTFOUND_VALUE = 999999;

	m_indices.reserve(444000);
	float distance = GetPsSetting_Float("connection_max_distance", 0.35f);
	if (distance == 0) return;
	const float distanseSqr = distance * distance;

	m_indices_alt.reserve(m_count * m_max_connections);
	m_length_alt.reserve(m_count * m_max_connections);

	for (unsigned int i = 0; i < m_count * m_max_connections; ++i)
	{
		m_indices_alt.push_back(particle_data_init::EMPTY_VALUE);
		m_length_alt.push_back(123456.0f); // don't care
	}

	auto randomIndexes = std::vector<unsigned int>(m_count);
	std::generate(randomIndexes.begin(), randomIndexes.end(), UniqueNumber);
	std::random_device rd;
	std::mt19937 g(rd());
	std::shuffle(randomIndexes.begin(), randomIndexes.end(), g);

	unsigned int overflowedConnections = 0;
	unsigned int overflowedConnectionsA = 0;
	for (unsigned int ir = 0; ir < m_count; ++ir)
	{
		unsigned int ia = randomIndexes[ir];
		// find empty spot in a
		unsigned int relativePosMoverA = NOTFOUND_VALUE;
		for (unsigned int mini_it = 0; mini_it < m_max_connections; ++mini_it)
		{
			if (m_indices_alt[ia*m_max_connections + mini_it] == particle_data_init::EMPTY_VALUE){
				relativePosMoverA = mini_it;
				break;
			}
		}
		if (relativePosMoverA == NOTFOUND_VALUE){
			overflowedConnectionsA += 1;
			continue;
		}
		// Found an empty spot on A here, lets find something in B
		for (unsigned int ib = 0; ib < m_count; ++ib)
		{
			if (ia == ib) continue;
			auto len2 = glm::length2(m_pos[ia] - m_pos[ib]);
			if (len2 > distanseSqr) continue;


			// find empty spot in b
			unsigned int absoluteEmptySpotB = NOTFOUND_VALUE;
			for (unsigned int mini_it = 0; mini_it < m_max_connections; ++mini_it)
			{
				if (m_indices_alt[ib*m_max_connections + mini_it] == particle_data_init::EMPTY_VALUE){
					absoluteEmptySpotB = ib*m_max_connections + mini_it;
					break;
				}
			}
			if (absoluteEmptySpotB == NOTFOUND_VALUE){
				overflowedConnections += 1;
				continue;
			}
			// Found an empty B here
			auto len = sqrt(len2);
			m_indices_alt[ia*m_max_connections + relativePosMoverA] = ib;
			m_length_alt[ia*m_max_connections + relativePosMoverA] = len;

			m_indices_alt[absoluteEmptySpotB] = ia;
			m_length_alt[absoluteEmptySpotB] = len;

			m_indices.push_back(ia);
			m_indices.push_back(ib);

			++relativePosMoverA;
			if (relativePosMoverA >= m_max_connections) break;

		}
	}
	std::cout << "overflowedConnections: " << overflowedConnections << "\n";
	std::cout << "overflowedConnectionsA: " << overflowedConnectionsA << "\n";
	m_connection_count = m_indices.size();

	std::cout << "m_indices_alt [";
	for (unsigned int i = 0; i < m_indices_alt.size(); i += 1){
		if (i%m_max_connections == 0){
			std::cout << "\n   ";
			if (i > 20){
				std::cout << "...";
				break;
			}
		}
		std::cout << m_indices_alt[i] << " ";
	}
	std::cout << "\n]\n";
}



void particle_data_init::initialize_buffers_from_data(particle_data& particle_data_ref)
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

	if (particle_data_ref.CONNECTION_COUNT > 0){
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, particle_data_ref.buffer[connection_index]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), &m_indices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_TEXTURE_BUFFER, particle_data_ref.buffer[connection_index_alt]);
		glBufferData(GL_TEXTURE_BUFFER, m_indices_alt.size() * sizeof(unsigned int), &m_indices_alt[0], GL_DYNAMIC_DRAW);

		glBindBuffer(GL_TEXTURE_BUFFER, particle_data_ref.buffer[connection_length_alt]);
		glBufferData(GL_TEXTURE_BUFFER, m_length_alt.size() * sizeof(float), &m_length_alt[0], GL_DYNAMIC_DRAW);
	}

}


void particle_data_init::doAllTheInitisation(particle_data& particle_data_ref)
{
	//load_positions_from_csv();
	auto loadFile = GetPsSetting_String("load_this_file_instead", "");
	if (loadFile != "")
		load_positions_from_model(loadFile.c_str());
	else
	{
		m_count = GetPsSetting_Int("count", 100);
		if (GetPsSetting_Bool("use_structured_spawning", true))
			this->generate_positions_structured();
		else
			this->generate_positions_random();
	}
	generate_colors_sinus();
	//generate_colors_gradient();
	generate_velocities_random();
	generate_indexes();

	initialize_buffers_from_data(particle_data_ref);
}
