#ifndef __CHUNKHANDLER_H__
#define __CHUNKHANDLER_H__
#include <GL/glfw3.h>
#include <vector>
#include "chunk.h"

class ChunkHandler
{
public:
	ChunkHandler();
	~ChunkHandler();
	
	void Render();
	
//private:
	std::vector<Chunk*> ChunkList;
	GLuint textureHandle;
	GLuint shaderProgramHandle;
	GLint viewMatrixLocation;
	GLint modelMatrixLocation;
};
#endif
