#ifndef __GUIBASE_H__
#define __GUIBASE_H__

namespace KyubuGui
{
	class GuiBase
	{
	public:
		GuiBase();
		virtual ~GuiBase();

		virtual void Update();
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

		virtual void OnLeftMousePress(int x, int y);
		virtual void OnRightMousePress(int x, int y);
		virtual void OnMiddleMousePress(int x, int y);

		virtual void OnLeftMouseRelease(int x, int y);
		virtual void OnRightMouseRelease(int x, int y);
		virtual void OnMiddleMouseRelease(int x, int y);

		virtual void OnLeftMouseClick(int x, int y);
		virtual void OnRightMouseClick(int x, int y);
		virtual void OnMiddleMouseClick(int x, int y);

		virtual void OnScrollUp();
		virtual void OnScrollDown();

		virtual void OnKeyDown();
		virtual void OnKeyUp();

	protected:
	private:
		int x;
		int y;
		int width;
		int height;
	};
}

#endif
