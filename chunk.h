#ifndef __CHUNK_H__
#define __CHUNK_H__
#include <GL/glfw3.h>

class Chunk
{
public:
	Chunk(GLuint shaderProgramHandle);
	~Chunk();
	
	void Update();
	void Render();

	short chunkX;
	short chunkY;
//private:
	GLuint vaoHandle;
	int triangleCount;
	unsigned short* blockData;
};

#endif
