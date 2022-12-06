#pragma once

#include <vector>
#include <glm/glm.hpp>

#include <GL/glew.h>

using namespace glm;

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

	unsigned int CONNECTION_COUNT = -1;
	unsigned int COUNT = 100000;
};


// for initialisation work (pretty dirt struct)
// Each array has it's own type and therefore are difficult to keep structured
class particle_data_init
{
public:
	virtual ~particle_data_init() {
		clean();
	}

	void doAllTheInitisation(particle_data& particle_data_ref);
	void clean() {
		delete m_pos;
		delete m_col;
		delete m_vel;
		m_indices.clear();
		m_indices_alt.clear();
		m_length_alt.clear();
	}
	static const unsigned int EMPTY_VALUE = 108108U;
	static const unsigned int m_max_connections = 20;

private:
	vec4* m_pos = nullptr;
	vec4* m_col = nullptr;
	vec4* m_vel = nullptr;
	unsigned int m_count = 0;

	std::vector<unsigned int> m_indices; // to draw connections
	std::vector<unsigned int> m_indices_alt;
	std::vector<float> m_length_alt;
	unsigned int m_connection_count = 0;

	void generate_colors_random();
	void generate_colors_sinus();
	void generate_colors_gradient();
	void generate_positions_random();
	void generate_positions_structured();
	void transform_positions(const mat4x4& mat, long count);
	void load_positions_from_csv();
	void load_positions_from_model(const char* file_name);
	void generate_velocities_random();
	void generate_indexes();

	void initialize_buffers_from_data(particle_data& particle_data_ref);
};
