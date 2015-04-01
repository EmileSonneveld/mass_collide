#pragma once

#include <stdio.h>

#include <glm/glm.hpp>

#define printOpenGLError() printOglError(__FILE__, __LINE__)
int printOglError(const char *file, const int line);

extern double g_cursor_x;
extern double g_cursor_y; // get set by particle smasher

glm::vec4 CursorToWorldspace(float depth);
