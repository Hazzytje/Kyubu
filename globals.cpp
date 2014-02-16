#include <GL/glew.h>
#include <GL/glfw3.h>
#include <iostream>

#include "globals.h"

namespace Globals
{
	void PrintAllGlErrors()
	{
		GLenum error = glGetError();
		while(error != GL_NO_ERROR)
		{
			std::cout << "glError: " << std::hex << error << "\n" << std::flush;
			error = glGetError();
		}
	}
	Game* gameInstance;
	Game& getGameInstance()
	{
		return *Globals::gameInstance;
	}
}
