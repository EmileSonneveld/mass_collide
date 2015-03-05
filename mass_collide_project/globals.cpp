#include "globals.h"
#include <string>

#if defined(_WIN32) || defined(WIN32)

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#else
// don't print to IDE on linux
void OutputDebugStringA(const char *file){}
#endif


int printOglError(const char *file, const int line)
{
	GLenum glErr;
	int    retCode = 0;

	glErr = glGetError();
	if (glErr != GL_NO_ERROR)
	{
		printf("glError in file %s @ line %d: %s\n",
			file, line, gluErrorString(glErr));
		OutputDebugStringA(file);
		OutputDebugStringA(": ");
		OutputDebugStringA(std::to_string(line).c_str());
		OutputDebugStringA(": ");
		OutputDebugStringA((char*)gluErrorString(glErr));
		OutputDebugStringA("\n");
		retCode = 1;
	}
	return retCode;
}

