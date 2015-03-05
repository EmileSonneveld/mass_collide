#ifndef SHADER_HPP
#define SHADER_HPP

#include <string>

std::string getCodeFromFile(const char * file_path);
GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path);
GLuint LoadComputeShader(const char * compute_file_path);
#endif
