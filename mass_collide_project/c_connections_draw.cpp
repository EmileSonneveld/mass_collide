#include "c_connections_draw.h"

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <string>
#include <iostream>
#include <fstream>
#include <assert.h>
#include <random>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/norm.hpp>
using namespace glm;

#include <common/shader.hpp>
#include <common/texture.hpp>
#include <common/controls.hpp>

#include "particle_data.h"
#include "globals.h"

void c_connections_draw::initialize()
{
	m_program = LoadShaders("rc/simple_vert.glsl", "rc/simple_frag.glsl");
	printOpenGLError();
	m_uniform_matrix = glGetUniformLocation(m_program, "ViewProjectionMatrix");
	m_in_attrib_position = glGetAttribLocation(m_program, "inPosition");
	m_in_attrib_color = glGetAttribLocation(m_program, "inColor");
}

void c_connections_draw::process(particle_data& particle_data_ref)
{
	glUseProgram(m_program);
	printOpenGLError();

	glCullFace(GL_FRONT_AND_BACK);

	glm::mat4 ViewProjectionMatrix = getProjectionMatrix() * getViewMatrix();
	glUniformMatrix4fv(m_uniform_matrix, 1, GL_FALSE, &ViewProjectionMatrix[0][0]);

	glEnableVertexAttribArray(m_in_attrib_position);
	glBindBuffer(GL_ARRAY_BUFFER, particle_data_ref.buffer[position]);
	glVertexAttribPointer(m_in_attrib_position, 4, GL_FLOAT, GL_FALSE, 0, nullptr);

	glEnableVertexAttribArray(m_in_attrib_color);
	glBindBuffer(GL_ARRAY_BUFFER, particle_data_ref.buffer[color]);
	glVertexAttribPointer(m_in_attrib_color, 4, GL_FLOAT, GL_FALSE, 0, nullptr);


	//glBindBuffer(GL_ARRAY_BUFFER, particle_data_ref.buffer[position]);
	printOpenGLError();
	// Index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, particle_data_ref.buffer[connection_index]);
	printOpenGLError();

	glDrawElements(
		GL_LINES,
		particle_data_ref.CONNECTION_COUNT,
		GL_UNSIGNED_INT,
		(void*)0
		);
	printOpenGLError();

}

void c_connections_draw::clean()
{
	if (m_program)
		glDeleteProgram(m_program);
	m_program = 0;
}
