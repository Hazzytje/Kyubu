#ifndef GLWRAP_H
#define GLWRAP_H

#include <string>
#include <GL/glew.h>

namespace GlWrap
{
	class GlProgram
	{
	public:
		GlProgram();
		~GlProgram();
		void AttachShader(GLuint shader);
		void DetachShader(GLuint shader);
		void Link();
		void Use();
		GLint GetAttribLocation(std::string name);
		void BindFragDataLocation(GLuint colorNumber, std::string name);
		GLuint glProgram;
	};
}

#endif
