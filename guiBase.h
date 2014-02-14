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
		virtual void SetY(int newY) {y = newY;}
		virtual int Width() {return width;}
		virtual int Height() {return height;}
		virtual void SetWidth(int newWidth) {width = newWidth;}
		virtual void SetHeight(int newHeight) {height = newHeight;}
		virtual void SetPos(int newX, int newY) {SetX(newX); SetY(newY);}
		virtual void SetSize(int newWidth, int newHeight)
			{SetWidth(newWidth); SetHeight(newHeight);}
	protected:
	private:
		int x;
		int y;
		int width;
		int height;
	};
}

#endif
