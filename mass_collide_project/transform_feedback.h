#pragma once

#include <GL/glew.h>

struct particle_data;

class transform_feedback
{
public:
	transform_feedback():
		m_program(0)
	{}
	virtual ~transform_feedback(){}

	void initialize();

	//void SetSourceDataAndGenerateSwapBuffer(GLint source);
	GLint DoTheCalculation(particle_data& particle_data_ref); // returns the new buffer

	void clean();


private:

	GLint m_program;
	static const GLint m_in_attrib_position = 0; //defined in shader
	static const GLint m_in_attrib_velocity = 1; //defined in shader

};


GLint CompileComputeShader(const GLchar* filename);
