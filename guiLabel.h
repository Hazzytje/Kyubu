#ifndef __GUILABEL_H__
#define __GUILABEL_H__

#include "guiBase.h"
#include <string>
#include "textrenderer.h"

namespace KyubuGui
{
	class GuiLabel : public GuiBase
	{
	public:
		GuiLabel();
		GuiLabel(const std::string& str);
		void setText(const std::string& str);
		std::string getText();
		virtual void Render() override;
		virtual ~GuiLabel();
	protected:
	private:
		std::string str;
		TextRenderer textRenderer;
	};
}
#endif
