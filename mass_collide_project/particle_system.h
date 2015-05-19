#pragma once

#include <GL/glew.h>

struct particle_data;

class particle_system
{
public:
	particle_system() :
		m_buffer_billboard_vertex(0),
        m_shader_program(0),
        m_in_attrib_square(0),
        m_in_attrib_position(1),
        m_in_attrib_color(2),
		m_uniform_CameraRight_worldspace(0),
		m_uniform_CameraUp_worldspace(0),
		m_uniform_ViewProjMatrix(0),
        m_uniform_TextureSampler(0),
		m_texture(0)
	{}
	virtual ~particle_system(){}

	void initialize();
	void draw(particle_data& particle_data_ref);
	void clean();

private:
	GLuint m_buffer_billboard_vertex;

	GLuint m_shader_program;
	GLint m_in_attrib_square = 0; //defined in shader
	GLint m_in_attrib_position = 1; //defined in shader
	GLint m_in_attrib_color = 2; //defined in shader

	GLuint m_uniform_CameraRight_worldspace;
	GLuint m_uniform_CameraUp_worldspace;
	GLuint m_uniform_ViewProjMatrix;
	GLuint m_uniform_TextureSampler;

	GLuint m_texture;
};

