#include "guiLabel.h"
#include <mutex>
#include "globals.h"
namespace KyubuGui
{
	GuiLabel::GuiLabel(const std::string& str)
	:str(str)
	{
		textRenderer.AddText(str, GuiBase::X(), GuiBase::Y());
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
		textRenderer.Render();
	}

	void GuiLabel::SetPos(int newX, int newY)
	{
		LELERRORS
		GuiBase::SetPos(newX, newY);
		LELERRORS
		textRenderer.Clear();
		LELERRORS
		textRenderer.AddText(str, GuiBase::X(), GuiBase::Y());
		LELERRORS
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
