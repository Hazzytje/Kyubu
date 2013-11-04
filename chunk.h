#ifndef __CHUNK_H__
#define __CHUNK_H__
#include <GL/glfw3.h>

#include "chunkHandler.h"

class Chunk
{
public:
	Chunk();
	~Chunk();
	
	void Update();
	void Render();
	
	void RebuildVBOEBO();
	
	short SetBlockAt(unsigned char x, unsigned char y, unsigned char z, short blockId);
	short GetBlockAt(unsigned char x, unsigned char y, unsigned char z);

	short chunkX;
	short chunkY;
//private:
	GLuint vbo, ebo;
	GLuint alphaVbo, alphaEbo;
	GLuint vaoHandle;
	GLuint alphaVaoHandle;
	int triangleCount;
	int alphaTriangleCount;
	unsigned short* blockData;
	unsigned char* blockMeta;
	unsigned char* blockBiomes;
};

#endif
