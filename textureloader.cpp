#include "textureloader.h"
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

Loadedimage::Loadedimage(const char* fileName)
{
	FILE* file = fopen(fileName, "r");
	if(file == NULL)
	{
		printf("Error while opening file %s\n", fileName);
	}
	char header1 = fgetc(file);
	char header2 = fgetc(file);
	if(!(header1 == 'B' && header2 == 'M'))
	{
		printf("File header isn't BM in file %s\n", fileName);
	}
	int fileSize;
	fread(&fileSize, sizeof(int), 1, file);
	printf("File size: %i \n", fileSize);
	int unused;
	fread(&unused, sizeof(int), 1, file);
	int pixelArrayOffset;
	fread(&pixelArrayOffset, sizeof(int), 1, file);	

	int dibHeaderSize;
	fread(&dibHeaderSize, sizeof(int), 1, file);
	fread(&width, sizeof(int), 1, file);
	fread(&height, sizeof(int), 1, file);

	int rowPadding = 4 - ((width * 3) % 4);
	if(rowPadding == 4) { rowPadding = 0; }

	fseek(file, pixelArrayOffset, SEEK_SET);
	
	pixelData = new unsigned char[width * height * 4];
	for(int currentHeight = 0; currentHeight < height; currentHeight++)
	{
		for (int currentWidth = 0; currentWidth < width; currentWidth++)
		{
			unsigned char B = fgetc(file);
			unsigned char G = fgetc(file);
			unsigned char R = fgetc(file);
			pixelData[( height - currentHeight - 1) * width * 4 + currentWidth * 4 + 0] = R;
			pixelData[( height - currentHeight - 1) * width * 4 + currentWidth * 4 + 1] = G;
			pixelData[( height - currentHeight - 1) * width * 4 + currentWidth * 4 + 2] = B;
			pixelData[( height - currentHeight - 1) * width * 4 + currentWidth * 4 + 3] = 255;
			fgetc(file);
		}
		for (int i = 0; i < rowPadding; i++)
		{
			fgetc(file);
		}
	}

	fclose(file);
}

Loadedimage::~Loadedimage()
{
	delete[] pixelData;
}
