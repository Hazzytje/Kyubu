#ifndef __CHUNK_H__
#define __CHUNK_H__
#include <GL/glfw3.h>

class ChunkHandler;
#include "chunkHandler.h"

class Chunk
{
public:
	Chunk();
	~Chunk();
	
	void Update();
	void Render();
	
	void RebuildVBOEBO();

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
