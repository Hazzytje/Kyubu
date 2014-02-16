#include "gui.h"
#include "guiLabel.h"
#include <algorithm>

namespace KyubuGui
{
	Gui::Gui()
	{
		guiObjects.push_back(new GuiLabel("test"));
	}

	Gui::~Gui()
	{

	}

	Gui& Gui::GetInstance()
	{
		static Gui instance;
		return instance;
	}

	void Gui::AddObject(GuiBase& obj)
	{
		GetInstance().guiObjects.push_back(&obj);
	}

	void Gui::RemoveObjetct(GuiBase& obj)
	{
		auto& objs = GetInstance().guiObjects;
		auto it  = std::find(objs.begin(), objs.end(), &obj);
		if (it != objs.end()) //check if it exists in the vector
		{
			objs.erase(it);
		}
	}

	void Gui::Update()
	{
		for (auto guiObj : GetInstance().guiObjects)
		{
			guiObj->Update();
		}
	}

	void Gui::Render()
	{
		for (auto guiObj : GetInstance().guiObjects)
		{
			guiObj->Render();
		}
	}
}
