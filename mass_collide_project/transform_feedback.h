#pragma once

#include <GL/glew.h>

class transform_feedback
{
public:
	transform_feedback():
		m_program(0),
		m_buffer_velocity(0),
		m_buffer_result(0),
		m_buffer_input(0),
		m_buffer_size(0)
	{}
	virtual ~transform_feedback(){}

	void initialize();

	void SetSourceDataAndGenerateSwapBuffer(GLint source);
	GLint DoTheCalculation(); // returns the new buffer

	void clean();


private:

	GLint m_program;
	static const GLint m_in_attrib_position = 0; //defined in shader
	static const GLint m_in_attrib_velocity = 1; //defined in shader

	GLuint m_buffer_velocity; // owner

	GLuint m_buffer_result; // half owner, DIRTY!
	GLuint m_buffer_input; // needs to become the original vertexbuffer
	int m_buffer_size;

};


GLint CompileComputeShader(const GLchar* filename);
