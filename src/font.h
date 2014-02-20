#ifndef FONT_H
#define FONT_H

#include <freetype-gl.h>

#include <string>

class Font
{
public:
	Font(int fontSize, std::string filename);
	~Font();
	void LoadGlyphs(const wchar_t* text);

	texture_atlas_t* atlas;
	texture_font_t* font;
};

#endif
