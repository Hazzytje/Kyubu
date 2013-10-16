#ifndef __CHUNKHANDLER_H__
#define __CHUNKHANDLER_H__
#include <GL/glfw3.h>
#include <vector>
#include <queue>
#include <mutex>

class Chunk;
#include "chunk.h"

struct ChunkCache
{
	short x;
	short y;
	int compressedLength;
};

class ChunkHandler
{
public:
    ChunkHandler();
    ~ChunkHandler();
	void Update();
    void Render();
    ushort GetBlockAt(int x, int y, int z);
    
	static GLuint shaderProgramHandle;
	
//private:
    static std::vector<Chunk*> ChunkList;
    static std::queue<unsigned char*> chunkCache;
    static std::queue<ChunkCache> chunkCacheMeta;
    static std::mutex chunkCacheMutex;
    GLuint textureHandle;
    //GLuint shaderProgramHandle;
    GLint viewMatrixLocation;
    GLint modelMatrixLocation;
};
#endif
