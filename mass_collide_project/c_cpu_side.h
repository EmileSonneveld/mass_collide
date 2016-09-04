#pragma once

#include <vector>
#include <glm/glm.hpp>
using namespace glm;
#include <GL/glew.h>

#include "particle_data.h"

class c_cpu_side
{
public:
	c_cpu_side()
	{
	}
	virtual ~c_cpu_side()
	{
	}
	void init();
	void pick_point(particle_data& particle_data_ref);
	void update_point(particle_data& particle_data_ref);
	void drop_point();

private:
	uint m_point_index = 1;
	float m_point_depth = 0.5f;
};

