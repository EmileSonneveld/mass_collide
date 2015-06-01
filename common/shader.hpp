#ifndef SHADER_HPP
#define SHADER_HPP

#include <string>

std::string getCodeFromFile(const char * file_path);
GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path);
GLuint LoadShaderWithTransformFeedback(const GLchar* filename);
GLuint LoadTemporaryShader(const char * glsl_file_path, GLenum kind = GL_VERTEX_SHADER); // or GL_FRAGMENT_SHADER
#endif
