#include <stdio.h>
#include <zlib.h>

#include <string.h>
#include <cstdio>

#include "matrix.h"
#include "globals.h"
#include "hgl.h"
#include "chunkHandler.h"
#include "textureloader.h"

#define CHUNK_LAYER (16 * 16)
#define CHUNK_TOTALSIZE (CHUNK_LAYER * 256)

GLuint ChunkHandler::shaderProgramHandle = 0;
std::vector<Chunk*> ChunkHandler::ChunkList = std::vector<Chunk*>();
std::queue<unsigned char*> ChunkHandler::chunkCache = std::queue<unsigned char*>();
std::queue<ChunkCache> ChunkHandler::chunkCacheMeta = std::queue<ChunkCache>();
std::mutex ChunkHandler::chunkCacheMutex;

void ChunkHandler::Update()
{
	chunkCacheMutex.lock();
	if(chunkCache.size() > 0)
	{
		printf("adding chunk from network \\o/\n");
		
		Chunk* newChunk = new Chunk();
		
		unsigned char* zlibBuffer = chunkCache.front();
		ChunkCache meta = chunkCacheMeta.front();
		
		newChunk->blockData = new unsigned short[16 * 16 * 256];
		newChunk->chunkX = meta.x;
		newChunk->chunkY = meta.y;
		
		int size = CHUNK_TOTALSIZE * 3 + CHUNK_LAYER;
		uLongf outsize = size;

		unsigned char buffer[CHUNK_TOTALSIZE * 3 + CHUNK_LAYER];
		uncompress(buffer, &outsize, zlibBuffer, meta.compressedLength);

		memcpy(newChunk->blockData, buffer, CHUNK_TOTALSIZE * 2);
		//memcpy(this->Data_BlockMeta, buffer + CHUNK_TOTALSIZE * 2, CHUNK_TOTALSIZE);
		//memcpy(this->Data_BlockBiomes, buffer + CHUNK_TOTALSIZE * 3, CHUNK_LAYER);
		
		newChunk->RebuildVBOEBO();
		
		ChunkList.push_back(newChunk);
		
		chunkCache.pop();
		chunkCacheMeta.pop();
	}
	chunkCacheMutex.unlock();
}

ChunkHandler::ChunkHandler()
{
    //shaders and shaderprogram
    GLuint vertexShader = hgl::shaders::CompileShader("vertShader.txt", GL_VERTEX_SHADER);
    GLuint fragmentShader = hgl::shaders::CompileShader("fragShader.txt", GL_FRAGMENT_SHADER);

    shaderProgramHandle = glCreateProgram();
    glAttachShader(shaderProgramHandle, vertexShader);
    glAttachShader(shaderProgramHandle, fragmentShader);
    glBindFragDataLocation(shaderProgramHandle, 0, "outColor");

    glLinkProgram(shaderProgramHandle);
    glUseProgram(shaderProgramHandle);

    //texture
    glGenTextures(1, &textureHandle);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureHandle);
    auto image = Loadedimage("terrain.bmp");

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glGenerateMipmap(GL_TEXTURE_2D);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.width, image.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.pixelData);

    glUniform1i(glGetUniformLocation(shaderProgramHandle, "tex"), 0);

    viewMatrixLocation = glGetUniformLocation(shaderProgramHandle, "view");
    modelMatrixLocation = glGetUniformLocation(shaderProgramHandle, "model");

    GLint projMatrixLocation = glGetUniformLocation(shaderProgramHandle, "proj");
    Matrix proj = Matrix::CreatePerspective(float(3.1415f * 0.5f), 1024.0f / 768.0f, 0.1f, 1000.0f);

    const float* projMatrix = proj.ToFloatArray();

    glUniformMatrix4fv(projMatrixLocation, 1, GL_FALSE, projMatrix);
    /*
    for(int x = 0; x < 1; x++)
    {
        for(int y = 0; y < 1; y++)
        {
            printf("chunk x: %i y: %i \n", x, y);
            Chunk* testChunk = new Chunk();
            testChunk->chunkX = x;
            testChunk->chunkY = y;

            ChunkList.push_back(testChunk);
        }
    }
*/
    glUseProgram(0);
}

ChunkHandler::~ChunkHandler()
{

}

void ChunkHandler::Render()
{
    //Globals::PrintAllGlErrors();
    //printf("at file %s, line %i\n", __FILE__, __LINE__);
    glUseProgram(shaderProgramHandle);
    //Globals::PrintAllGlErrors();
    //printf("at file %s, line %i\n", __FILE__, __LINE__);
    glActiveTexture(GL_TEXTURE0);
    //Globals::PrintAllGlErrors();
    //printf("at file %s, line %i\n", __FILE__, __LINE__);
    glBindTexture(GL_TEXTURE_2D, textureHandle);
    //Globals::PrintAllGlErrors();
    //printf("at file %s, line %i\n", __FILE__, __LINE__);
    Globals::getGameInstance().getPlayer().camera.InsertViewMatrix(viewMatrixLocation);
    //Globals::PrintAllGlErrors();
    //printf("at file %s, line %i\n", __FILE__, __LINE__);
for(auto chunk : ChunkList)
    {
        Matrix modelMatrix = Matrix::CreateTranslation(chunk->chunkX * 16, chunk->chunkY * 16, 0);
//		modelMatrix.DebugPrint();

        glUniformMatrix4fv(modelMatrixLocation, 1, GL_TRUE, modelMatrix.ToFloatArray());
        chunk->Render();
    }
    Globals::PrintAllGlErrors();
    //printf("at file %s, line %i\n", __FILE__, __LINE__);
    glBindVertexArray(0);
    glUseProgram(0);
}

ushort GetBlockAt(int x, int y, int z)
{
	if(z < 0 || z > 256)
	{
		return 0;
	}
	int neededChunkX = x - x % 16;
	int neededChunkY = y - y % 16;
	for(auto chunk : ChunkList)
	{
		if(chunk->chunkX == neededChunkX && chunk->chunkY == neededChunkY)
		{
			return chunk->blockData[z * 256 + y * 16 + x];
		}
	}
}




