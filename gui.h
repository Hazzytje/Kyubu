#ifndef GUI_H
#define GUI_H

#include <vector>
#include "guiBase.h"
namespace KyubuGui{
	class Gui
	{
	public:
		~Gui();

		static Gui& GetInstance();

		static void AddObject(GuiBase& obj);
		static void RemoveObjetct(GuiBase& obj);
		static void Update();
		static void Render();

	private:
		Gui();
		std::vector<GuiBase*> guiObjects;
	};
}

#endif
