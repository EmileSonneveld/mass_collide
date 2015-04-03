#pragma once

#include <string>

#include <GL/glew.h>

struct particle_data;

class transform_feedback
{
public:
	transform_feedback() :
		m_program(0),
		m_uniform_point()
	{}
	virtual ~transform_feedback(){}

	void initialize(std::string file_name = "compute.glsl");
	GLint ProccesPositions(particle_data& particle_data_ref);
	GLint ProccesVelocities(particle_data& particle_data_ref);
	void clean();

private:
	GLint m_program;
	GLint m_in_attrib_position = 0; //defined in shader
	GLint m_in_attrib_velocity = 1; //defined in shader

	GLint m_uniform_point;
};


GLint CompileComputeShader(const GLchar* filename);
