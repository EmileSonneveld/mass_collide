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

#include "c_cpu_side.h"
#include "globals.h"



void c_cpu_side::init()
{
}

void c_cpu_side::pick_point(particle_data& particle_data_ref)
{
	vec4* pos_arr = new vec4[particle_data_ref.COUNT];

	glBindBuffer(GL_TEXTURE_BUFFER, particle_data_ref.buffer[position]);
	glGetBufferSubData(GL_TEXTURE_BUFFER, 0, particle_data_ref.COUNT * sizeof(vec4), pos_arr);
	printOpenGLError();

	for (unsigned int i = 0; i < particle_data_ref.COUNT; i++)
	{

	}
	m_point_index = 0;
	m_point_depth = 0.5f;
}

void c_cpu_side::update_point(particle_data& particle_data_ref)
{
	glBindBuffer(GL_TEXTURE_BUFFER, particle_data_ref.buffer[position]);
	vec4* mapped_prt = (vec4*)glMapBufferRange(GL_TEXTURE_BUFFER, m_point_index, 1, GL_MAP_WRITE_BIT);
	printOpenGLError();
	m_point_depth = -2.f + (rand() % 100) / 100.0f * 4.0f;
	glm::vec4 pos = CursorToWorldspace(m_point_depth);
	mapped_prt[0] = pos;
	glUnmapBuffer(GL_TEXTURE_BUFFER);
}

void c_cpu_side::drop_point()
{
}
