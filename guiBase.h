#ifndef __GUIBASE_H__
#define __GUIBASE_H__

class GuiBase
{
	public:
		GuiBase();
		virtual ~GuiBase();
		virtual void Render() = 0;
	protected:
	private:
		int x;
		int y;
		int width;
		int height;
};

#endif
