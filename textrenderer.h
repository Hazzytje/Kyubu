#ifndef __TEXTRENDERER_H__
#define __TEXTRENDERER_H__

#include <string>
#include <vector>
#include <GL/glew.h>

#include <freetype-gl.h>
#include <texture-font.h>

#include "font.h"

class TextRenderer
{
	public:
		TextRenderer();
		~TextRenderer();
		
		void Render();
		void AddText(std::string text, int x, int y);
	protected:
	private:
		GLuint vao;
		GLuint vbo, ebo;
		GLuint shaderProgram;
		
		std::vector<float> vertices;
		std::vector<GLuint> indices;
		
		int elementCount;

		static Font& GetFont();
};

#endif
