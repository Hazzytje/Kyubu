#include "textrenderer.h"
#include "hgl.h"

#include "globals.h"
#include <iostream>

TextRenderer::TextRenderer()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	GetProgram().Use();

	GLint posAttrib = GetProgram().GetAttribLocation("position");
	glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);
	glEnableVertexAttribArray(posAttrib);

	GLint texCoordAttrib = GetProgram().GetAttribLocation("texCoord");
	glVertexAttribPointer(texCoordAttrib, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(texCoordAttrib);

	glUniform1i(glGetUniformLocation(GetProgram().glProgram, "tex"), 0);

	glBindVertexArray(0);
}

TextRenderer::~TextRenderer()
{
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);
	glDeleteVertexArrays(1, &vao);
}

void TextRenderer::Clear()
{
	vertices.clear();
	indices.clear();
	elementCount = 0;
}

void TextRenderer::Render()
{
	GetProgram().Use();
	glBindVertexArray(vao);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, GetFont().atlas->id);
	glDrawElements(GL_TRIANGLES, elementCount, GL_UNSIGNED_INT, 0);
	
	glBindVertexArray(0);
	glUseProgram(0);
}

void TextRenderer::AddText(std::string text, int x, int y)
{
	glBindVertexArray(vao);
	
	float penX = x;
	float penY = y;
	
	wchar_t prevChar = 0;
	
	for (unsigned int i = 0; i < text.length(); i++)
	{
		texture_glyph_t* glyph = texture_font_get_glyph(GetFont().font, text[i]);
		
		if(prevChar != 0)
		{
			penX += texture_glyph_get_kerning(glyph, prevChar);
		}
		
		int x0 = penX + glyph->offset_x;
		int y0 = penY + glyph->offset_y;
		int x1 = x0 + glyph->width;
		int y1 = y0 - glyph->height;
		
		float s0 = glyph->s0;
		float t0 = glyph->t0;
		float s1 = glyph->s1;
		float t1 = glyph->t1;
		
		vertices.push_back(x0); vertices.push_back(y0); vertices.push_back(0.0); vertices.push_back(s0); vertices.push_back(t0);
		vertices.push_back(x0); vertices.push_back(y1); vertices.push_back(0.0); vertices.push_back(s0); vertices.push_back(t1);
		vertices.push_back(x1); vertices.push_back(y1); vertices.push_back(0.0); vertices.push_back(s1); vertices.push_back(t1);
		vertices.push_back(x1); vertices.push_back(y0); vertices.push_back(0.0); vertices.push_back(s1); vertices.push_back(t0);
		
		int vertexCount = vertices.size() / 5;
		indices.push_back(vertexCount - 4); indices.push_back(vertexCount - 3); indices.push_back(vertexCount - 2); 
		indices.push_back(vertexCount - 4); indices.push_back(vertexCount - 2); indices.push_back(vertexCount - 1); 
		
		penX += glyph->advance_x;
		penY += glyph->advance_y;
		
		prevChar = text[i];
	}
	elementCount = indices.size();
	
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_DYNAMIC_DRAW);

	glBindVertexArray(0);
}

Font& TextRenderer::GetFont()
{
	static Font actualFont(16, "/usr/share/xbmc/media/Fonts/arial.ttf");;
	return actualFont;
}

GlWrap::GlProgram& TextRenderer::GetProgram()
{
	static GlWrap::GlProgram actualProgram;
	static bool initialized = false;
	if(!initialized)
	{
		actualProgram.AttachShader(hgl::shaders::CompileShader("textVertShader.txt", GL_VERTEX_SHADER));
		actualProgram.AttachShader(hgl::shaders::CompileShader("textFragShader.txt", GL_FRAGMENT_SHADER));
		actualProgram.BindFragDataLocation(0, "outColor");
		actualProgram.Link();
		initialized = true;
	}
	return actualProgram;
}
