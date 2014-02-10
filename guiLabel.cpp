#include "guiLabel.h"
#include <mutex>
namespace KyubuGui
{
	GuiLabel::GuiLabel(const std::string& str)
	:str(str)
	{
		//textRenderer.AddText(str, GuiBase::X(), GuiBase::Y());
	}
	
	GuiLabel::GuiLabel()
	:str("")
	{
		
	}

	GuiLabel::~GuiLabel()
	{
		
	}
	
	void GuiLabel::Render()
	{
		
	}
	
	void GuiLabel::setText(const std::string& newStr)
	{
		str = newStr;
	}
	
	std::string GuiLabel::getText()
	{
		return str;
	}
}