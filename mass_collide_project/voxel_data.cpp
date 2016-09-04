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

#include "voxel_data.h"



void voxel_data::init()
{
	const unsigned int vertCount = 4;

	vec4* m_pos = nullptr;
	if (m_pos == nullptr)
		m_pos = new vec4[vertCount];

	m_pos[0] = vec4(-1, -1, 0, 0);
	m_pos[1] = vec4(1, -1, 0, 0);
	m_pos[2] = vec4(-1, 1, 0, 0);
	m_pos[3] = vec4(1, 1, 0, 0);

	glBindBuffer(GL_TEXTURE_BUFFER, m_screenspace_rect_buffer);
	glBufferData(GL_TEXTURE_BUFFER, vertCount * sizeof(vec4), m_pos, GL_DYNAMIC_DRAW);


	vec4* m_norm = nullptr;
	if (m_norm == nullptr)
		m_norm = new vec4[vertCount];

	m_norm[0] = vec4(-1, -1, 0, 0);
	m_norm[1] = vec4(1, -1, 0, 0); 
	m_norm[2] = vec4(-1, 1, 0, 0);
	m_norm[3] = vec4(1, 1, 0, 0);
}
