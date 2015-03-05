#pragma once


#include <stdio.h>

#include <GL/glew.h>


//#if defined(_WIN32) || defined(WIN32)
#define printOpenGLError() printOglError(__FILE__, __LINE__)
//#else
//#define printOpenGLError() printOglError(__FILE__.c_str(), __LINE__.c_str())
//#endif

int printOglError(const char *file, const int line);