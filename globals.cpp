#include <GL/glew.h>
#include <GL/glfw3.h>
#include <stdio.h>

#include "globals.h"

namespace Globals
{
	void PrintAllGlErrors()
	{
		GLenum error = glGetError();
		while(error != GL_NO_ERROR)
		{
			printf("glError: %x\n", error);
			error = glGetError();
		}
	}
}
