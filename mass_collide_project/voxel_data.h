#pragma once

#include <vector>
#include <glm/glm.hpp>
using namespace glm;
#include <GL/glew.h>

class voxel_data
{
public:
	voxel_data()
	{
		glGenBuffers(1, &m_screenspace_rect_buffer);
	}
	virtual ~voxel_data()
	{
		glDeleteBuffers(1, &m_screenspace_rect_buffer);
	}
	void init();

private:
	GLuint m_screenspace_rect_buffer;

};

