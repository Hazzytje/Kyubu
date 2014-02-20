#ifndef __TEXTURELOADER_H__
#define __TEXTURELOADER_H__

class Loadedimage
{
public:
	Loadedimage(const char* fileName);
	~Loadedimage();
	int width;
	int height;
	unsigned char* pixelData;
};

#endif
