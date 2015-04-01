#pragma once

#include <stdio.h>

#include <GL/glew.h>


#define printOpenGLError() printOglError(__FILE__, __LINE__)
int printOglError(const char *file, const int line);