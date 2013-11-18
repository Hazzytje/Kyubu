#ifndef __TEXTRENDERER_H__
#define __TEXTRENDERER_H__

#include <string>

class TextRenderer
{
	public:
		TextRenderer();
		~TextRenderer();
		
		void Render();
		void AddText(std::string text, int x, int y);
	protected:
	private:
	
};

#endif
