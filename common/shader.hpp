#ifndef SHADER_HPP
#define SHADER_HPP

#include <string>
#include <GL/glew.h>

std::string getCodeFromFile(const char * file_path);
GLuint LoadShaderFromPath(const char * vertex_file_path, const char * fragment_file_path);
GLuint LoadShaderFromString(const std::string& vertex_file_path, const std::string& fragment_file_path);
GLuint LoadShaderWithTransformFeedbackFromPath(const char* filename);
GLuint LoadShaderWithTransformFeedbackFromString(const std::string& shader_text);
GLuint LoadTemporaryShader(const std::string&, GLenum kind = GL_VERTEX_SHADER); // or GL_FRAGMENT_SHADER
#endif
