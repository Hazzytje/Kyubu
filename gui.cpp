#include "gui.h"
#include "guiLabel.h"
#include <algorithm>
#include <math.h>

namespace KyubuGui
{
	Gui::Gui()
	{
		//testing purposes
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
		static double time = 0;
		time += M_PI / 60;
		GetInstance().guiObjects[0]->SetPos(200 + 100 * -cos(time), 200 + 100 * sin(time));
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
