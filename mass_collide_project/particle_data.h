#pragma once

#include <vector>
#include <glm/glm.hpp>
using namespace glm;

struct data_struct 
{
	GLuint buffer;
	void* cpuPtr;
	bool isBufferUpToDate;
	unsigned int size;
};

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
	unsigned int COUNT = 100000;
};

//class vec4;
// for initialisation work (pretty dirt struct)
// Each array has it's own type and therefore are difficult to keep structured
class data
{
public:
	data() :
		m_pos(nullptr),
		m_col(nullptr),
		m_vel(nullptr),
		m_count(0),
		m_connection_count(0)
	{
	}
	~data(){
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



