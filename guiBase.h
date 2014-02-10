#ifndef __GUIBASE_H__
#define __GUIBASE_H__

namespace KyubuGui
{
	class GuiBase
	{
	public:
		GuiBase();
		virtual ~GuiBase();
		virtual void Render() = 0;
		virtual int X() {return x;}
		virtual int Y()	{return y;}
		virtual void SetX(int newX) {x = newX;}
		virtual void SetY(int newY)	{y = newY;}
	protected:
	private:
		int x;
		int y;
		int width;
		int height;
	};
}

#endif
