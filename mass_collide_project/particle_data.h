#pragma once

#include <vector>
#include <glm/glm.hpp>
using namespace glm;
#include <GL/glew.h>

// add an element to this enum to use a shared
enum bufferName
{
	swap,
	position,
	color,
	velocity,
	connection_index,
	connection_index_alt,
	connection_length_alt,
	size_element
};

struct particle_data
{
	particle_data()
	{
		glGenBuffers(size_element, &buffer[0]);
	}
	void clean()
	{
		glDeleteBuffers(size_element, &buffer[0]);
	}

	GLuint buffer[size_element];

	unsigned int CONNECTION_COUNT;
	//static const unsigned int MAX_CONNECTIONS = particle_data_init::m_max_connections;
	unsigned int COUNT = 100000;
};

//class vec4;
// for initialisation work (pretty dirt struct)
// Each array has it's own type and therefore are difficult to keep structured
class particle_data_init
{
public:
	particle_data_init() :
		m_pos(nullptr),
		m_col(nullptr),
		m_vel(nullptr),
		m_count(0),
		m_connection_count(0)
	{
	}
	~particle_data_init(){
		clean();
	}

	void doAllTheInitisation(particle_data& particle_data_ref);
	void clean(){
		delete m_pos;
		delete m_col;
		delete m_vel;
		m_indices.clear();
		m_indices_alt.clear();
		m_length_alt.clear();
	}
	static const unsigned int EMPTY_VALUE = 108108U;
	static const unsigned int m_max_connections = 30;

private:
	vec4* m_pos;
	vec4* m_col;
	vec4* m_vel;
	unsigned int m_count;

	std::vector<unsigned int> m_indices;
	std::vector<unsigned int> m_indices_alt;
	std::vector<float> m_length_alt;
	unsigned int m_connection_count;

	void generate_colors_random();
	void generate_colors_sinus();
	void generate_colors_gradient();
	void generate_positions_random();
	void generate_positions_structured();
	void load_positions_from_model(const char* file_name);
	void generate_velocities_random();
	void generate_indexes();

	void initialize_buffers_from_data(particle_data& particle_data_ref);
};



