#pragma once

#include <GL/glew.h>
#include "transform_feedback.h"


class particle_system
{
public:
	particle_system() :
		m_buffer_billboard_vertex(0),
		m_buffer_position(0),
		m_buffer_color(0),
		m_shader_program(0),
		m_uniform_CameraRight_worldspace(0),
		m_uniform_CameraUp_worldspace(0),
		m_uniform_ViewProjMatrix(0),
		m_uniform_TextureSampler(0),
		m_compute_program(0),
		m_texture(0),
		m_transform_feedback_inst()
	{}
	virtual ~particle_system(){}

	void initialize();
	void tick();
	void clean();

private:
	GLuint m_buffer_billboard_vertex;

	GLuint m_buffer_position;
	GLuint m_buffer_color;


	GLuint m_shader_program;
	static const GLint m_in_attrib_square = 0; //defined in shader
	static const GLint m_in_attrib_position = 1; //defined in shader
	static const GLint m_in_attrib_color = 2; //defined in shader

	GLuint m_uniform_CameraRight_worldspace;
	GLuint m_uniform_CameraUp_worldspace;
	GLuint m_uniform_ViewProjMatrix;
	GLuint m_uniform_TextureSampler;

	GLuint m_compute_program;

	GLuint m_texture;


	transform_feedback m_transform_feedback_inst;

	static const unsigned int PARTICLES_COUNT = 100000;
};

