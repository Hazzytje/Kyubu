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
	GLuint shaderProgramHandle;
	GLuint vbo, ebo;
	GLuint vaoHandle;
	int triangleCount;
	unsigned short* blockData;
	unsigned char* blockMeta;
	unsigned char* blockBiomes;
};

#endif
