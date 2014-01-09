#ifndef __GUILABEL_H__
#define __GUILABEL_H__

#include "guiBase.h"
#include <string>

class GuiLabel : public GuiBase
{
	public:
		guiLabel(std::string str);
		virtual ~guiLabel();
	protected:
	private:
		std::string str;
};

#endif
