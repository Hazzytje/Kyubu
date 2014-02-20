#include "font.h"
#include <iostream>
#include "globals.h"

Font::Font(int fontSize, std::string filename)
{
	atlas = texture_atlas_new(512, 512, 1);
	font = texture_font_new_from_file( atlas, fontSize , filename.c_str());

	LoadGlyphs( L" !\"#$%&'()*+,-./0123456789:;<=>?"
				L"@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_"
				L"`abcdefghijklmnopqrstuvwxyz{|}~");
}

Font::~Font()
{
	texture_atlas_delete(atlas);
	texture_font_delete(font);
}

void Font::LoadGlyphs(const wchar_t* text)
{
	texture_font_load_glyphs(font, text);
}
