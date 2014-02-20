#include "glwrap.h"
#include <GL/glew.h>

namespace GlWrap
{
	GlProgram::GlProgram()
	{
		glProgram = glCreateProgram();
	}

	GlProgram::~GlProgram()
	{
		glDeleteProgram(glProgram);
	}

	void GlProgram::AttachShader(GLuint shader)
	{
		glAttachShader(glProgram, shader);
	}

	void GlProgram::DetachShader(GLuint shader)
	{
		glDetachShader(glProgram, shader);
	}

	void GlProgram::Link()
	{
		glLinkProgram(glProgram);
	}

	void GlProgram::Use()
	{
		glUseProgram(glProgram);
	}

	GLint GlProgram::GetAttribLocation(std::string name)
	{
		return glGetAttribLocation(glProgram, name.c_str());
	}

	void GlProgram::BindFragDataLocation(GLuint colorNumber, std::string name)
	{
		glBindFragDataLocation(glProgram, colorNumber, name.c_str());
	}
}
