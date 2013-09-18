#ifndef __HGL_H__
#define __HGL_H__

#include <GL/glew.h>

namespace hgl
{
	namespace shaders
	{
		GLuint CompileShader(const char* fileName, int shaderType);
	}
}

#endif
