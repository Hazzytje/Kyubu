#include <cstdlib>
#include <stdio.h>
#include <vector>
#include <GL/glew.h>
#include <GL/glfw3.h>
#include <cassert>
#include "blocks.h"
#include "globals.h"
#include "chunk.h"
#include "enums.h"

#define INDEX_OF_BLOCK(x, y, z) (((z) * 256) + ((y) * 16) + (x))

#define FRONT_LEFT_TOP 	   AddFrontLeftTopVertice  (vertexData, x, y, z);
#define FRONT_LEFT_BOTTOM  AddFrontLeftBotVertice  (vertexData, x, y, z);
#define FRONT_RIGHT_TOP    AddFrontRightTopVertice (vertexData, x, y, z);
#define FRONT_RIGHT_BOTTOM AddFrontRightBotVertice (vertexData, x, y, z);
#define BACK_LEFT_TOP      AddBackLeftTopVertice   (vertexData, x, y, z);
#define BACK_LEFT_BOTTOM   AddBackLeftBotVertice   (vertexData, x, y, z);
#define BACK_RIGHT_TOP     AddBackRightTopVertice  (vertexData, x, y, z);
#define BACK_RIGHT_BOTTOM  AddBackRightBotVertice  (vertexData, x, y, z);

inline void AddVertices(std::vector<float>& vertexData, int x, int y, int z)
{
	vertexData.push_back(x);
	vertexData.push_back(y);
	vertexData.push_back(z);
}

inline void AddFrontLeftTopVertice(std::vector<float>& vertexData, int x, int y, int z) {
	AddVertices(vertexData, x, y, z + 1);
}
inline void AddFrontLeftBotVertice(std::vector<float>& vertexData, int x, int y, int z) {
	AddVertices(vertexData, x, y, z);
}
inline void AddFrontRightTopVertice(std::vector<float>& vertexData, int x, int y, int z) {
	AddVertices(vertexData, x + 1, y, z + 1);
}
inline void AddFrontRightBotVertice(std::vector<float>& vertexData, int x, int y, int z) {
	AddVertices(vertexData, x + 1, y, z);
}
inline void AddBackLeftTopVertice(std::vector<float>& vertexData, int x, int y, int z) {
	AddVertices(vertexData, x, y + 1, z + 1);
}
inline void AddBackLeftBotVertice(std::vector<float>& vertexData, int x, int y, int z) {
	AddVertices(vertexData, x, y + 1, z);
}
inline void AddBackRightTopVertice(std::vector<float>& vertexData, int x, int y, int z) {
	AddVertices(vertexData, x + 1, y + 1, z + 1);
}
inline void AddBackRightBotVertice(std::vector<float>& vertexData, int x, int y, int z) {
	AddVertices(vertexData, x + 1, y + 1, z);
}

inline void AddIndices(unsigned int vertexCount, std::vector<GLuint>& indiceData)
{
	//triangle one
	indiceData.push_back(vertexCount - 4);
	indiceData.push_back(vertexCount - 3);
	indiceData.push_back(vertexCount - 2);

	//triangle two
	indiceData.push_back(vertexCount - 4);
	indiceData.push_back(vertexCount - 2);
	indiceData.push_back(vertexCount - 1);
}

inline float TextureCoordForBlock(short blockId, BlockSides side, byte texCoordIndex) //TODO: change texCoordIndex to an enum(?)
{
	assert(texCoordIndex > -1 && texCoordIndex < 4);
	if(texCoordIndex == 0 || texCoordIndex == 1)
	{
		return BlockInfo::texCoordArray[(blockId * 6 * 4) + static_cast<byte>(side) * 4 + texCoordIndex];
	}
	return BlockInfo::texCoordArray[(blockId * 6 * 4) + static_cast<byte>(side) * 4 + texCoordIndex] + TextureCoordForBlock(blockId, side, texCoordIndex - 2);
}

inline void AddBlockSide(int x, int y, int z, BlockSides side, std::vector<float>& vertexArray, std::vector<GLuint>& indiceArray, short blockId)
{
	switch (side)
	{
		case BlockSides::Top:   AddBackLeftTopVertice  (vertexArray, x, y, z); break;
		case BlockSides::Down:  AddFrontLeftBotVertice (vertexArray, x, y, z); break;
		case BlockSides::Left:  AddBackRightTopVertice (vertexArray, x, y, z); break;
		case BlockSides::Right: AddFrontLeftTopVertice (vertexArray, x, y, z); break;
		case BlockSides::Front: AddFrontRightTopVertice(vertexArray, x, y, z); break;
		case BlockSides::Back:  AddBackLeftTopVertice  (vertexArray, x, y, z); break;
	}
	vertexArray.push_back(TextureCoordForBlock(blockId, side, 0));
	vertexArray.push_back(TextureCoordForBlock(blockId, side, 1));
	
	switch (side)
	{
		case BlockSides::Top:   AddBackRightTopVertice (vertexArray, x, y, z); break;
		case BlockSides::Down:  AddFrontRightBotVertice(vertexArray, x, y, z); break;
		case BlockSides::Left:  AddBackLeftTopVertice  (vertexArray, x, y, z); break;
		case BlockSides::Right: AddFrontRightTopVertice(vertexArray, x, y, z); break;
		case BlockSides::Front: AddBackRightTopVertice (vertexArray, x, y, z); break;
		case BlockSides::Back:  AddFrontLeftTopVertice (vertexArray, x, y, z); break;
	}
	vertexArray.push_back(TextureCoordForBlock(blockId, side, 2));
	vertexArray.push_back(TextureCoordForBlock(blockId, side, 1));
	
	switch (side)
	{
		case BlockSides::Top:   AddFrontRightTopVertice(vertexArray, x, y, z); break;
		case BlockSides::Down:  AddBackRightBotVertice (vertexArray, x, y, z); break;
		case BlockSides::Left:  AddBackLeftBotVertice  (vertexArray, x, y, z); break;
		case BlockSides::Right: AddFrontRightBotVertice(vertexArray, x, y, z); break;
		case BlockSides::Front: AddBackRightBotVertice (vertexArray, x, y, z); break;
		case BlockSides::Back:  AddFrontLeftBotVertice (vertexArray, x, y, z); break;
	}
	vertexArray.push_back(TextureCoordForBlock(blockId, side, 2));
	vertexArray.push_back(TextureCoordForBlock(blockId, side, 3));
	
	switch (side)
	{
		case BlockSides::Top:   AddFrontLeftTopVertice (vertexArray, x, y, z); break;
		case BlockSides::Down:  AddBackLeftBotVertice  (vertexArray, x, y, z); break;
		case BlockSides::Left:  AddBackRightBotVertice (vertexArray, x, y, z); break;
		case BlockSides::Right: AddFrontLeftBotVertice (vertexArray, x, y, z); break;
		case BlockSides::Front: AddFrontRightBotVertice(vertexArray, x, y, z); break;
		case BlockSides::Back:  AddBackLeftBotVertice  (vertexArray, x, y, z); break;
	}
	vertexArray.push_back(TextureCoordForBlock(blockId, side, 0));
	vertexArray.push_back(TextureCoordForBlock(blockId, side, 3));
	unsigned int vertexCount = vertexArray.size() / 5;
	AddIndices(vertexCount, indiceArray);
}

void Chunk::RebuildVBOEBO()
{
	
	auto vertexData = std::vector<float>();
	vertexData.reserve(50000);
	auto indiceData = std::vector<GLuint>();
	indiceData.reserve(10000);
	
	auto alphaVertexData = std::vector<float>();
	alphaVertexData.reserve(5000);
	auto alphaIndiceData = std::vector<GLuint>();
	alphaIndiceData.reserve(1000);

	for(int x = 0; x < 16; x++)
	{
		for(int y = 0; y < 16; y++)
		{
			for(int z = 0; z < 256; z++)
			{
				if (blockData[INDEX_OF_BLOCK(x, y, z)] == 0)
					continue;
				
				unsigned short thisBlockId = blockData[INDEX_OF_BLOCK(x, y, z)];

				//booleans for storing what sides of the block will be rendered
				bool front = false, right = false, back = false, left = false, top = false, down = false;
				//back side of block
				if(x != 0)
				{
					unsigned short southBlockType = blockData[INDEX_OF_BLOCK(x - 1, y, z)];
					if(BlockInfo::GetHasAlpha(thisBlockId))
					{
						back = thisBlockId != southBlockType || BlockInfo::GetHasCustomCollision(thisBlockId);
					}
					else
					{
						if(southBlockType == 0 || BlockInfo::GetHasAlpha(southBlockType))
						{
							back = true;
						}
					}
				} // TODO: else check other chunks
				else { back = true; }
				//front side of block
				if(x != 15)
				{
					unsigned short northBlockType = blockData[INDEX_OF_BLOCK(x + 1, y, z)];
					if(BlockInfo::GetHasAlpha(thisBlockId))
					{
						front = thisBlockId != northBlockType || BlockInfo::GetHasCustomCollision(thisBlockId);
					}
					else
					{
						if(northBlockType == 0 || BlockInfo::GetHasAlpha(northBlockType))
						{
							front = true;
						}
					}
				}
				else { front = true; }
				//left side of block
				if(y != 15)
				{
					unsigned short westBlockType = blockData[INDEX_OF_BLOCK(x, y + 1, z)];
					if(BlockInfo::GetHasAlpha(thisBlockId))
					{
						left = thisBlockId != westBlockType || BlockInfo::GetHasCustomCollision(thisBlockId);
					}
					else
					{
						if(westBlockType == 0 || BlockInfo::GetHasAlpha(westBlockType))
						{
							left = true;
						}
					}
				}
				else { left = true; }
				//right side of block
				if(y != 0)
				{
					unsigned short eastBlockType = blockData[INDEX_OF_BLOCK(x, y - 1, z)];
					if(BlockInfo::GetHasAlpha(thisBlockId))
					{
						right = thisBlockId != eastBlockType || BlockInfo::GetHasCustomCollision(thisBlockId);
					}
					else
					{
						if(eastBlockType == 0 || BlockInfo::GetHasAlpha(eastBlockType))
						{
							right = true;
						}
					}
				}
				else { right = true; }
				//top side of block
				if(z != 255)
				{
					unsigned short topBlockType = blockData[INDEX_OF_BLOCK(x, y, z + 1)];
					if(BlockInfo::GetHasAlpha(thisBlockId))
					{
						top = thisBlockId != topBlockType || BlockInfo::GetHasCustomCollision(thisBlockId);
					}
					else
					{
						if(topBlockType == 0 || BlockInfo::GetHasAlpha(topBlockType))
						{
							top = true;
						}
					}
				}
				else { top = true; }
				//down side of block
				if(z != 0)
				{
					unsigned short botBlockType = blockData[INDEX_OF_BLOCK(x, y, z - 1)];
					if(BlockInfo::GetHasAlpha(thisBlockId))
					{
						down = thisBlockId != botBlockType || BlockInfo::GetHasCustomCollision(thisBlockId);
					}
					else
					{
						if(botBlockType == 0 || BlockInfo::GetHasAlpha(botBlockType))
						{
							down = true;
						}
					}
				}
				else {down = true;}
				
				if(BlockInfo::GetHasAlpha(thisBlockId))
				{
					if(back)  { AddBlockSide(x, y, z, BlockSides::Back,  alphaVertexData, alphaIndiceData, thisBlockId); }
					if(front) { AddBlockSide(x, y, z, BlockSides::Front, alphaVertexData, alphaIndiceData, thisBlockId); }
					if(right) { AddBlockSide(x, y, z, BlockSides::Right, alphaVertexData, alphaIndiceData, thisBlockId); }
					if(left)  { AddBlockSide(x, y, z, BlockSides::Left,  alphaVertexData, alphaIndiceData, thisBlockId); }
					if(top)   { AddBlockSide(x, y, z, BlockSides::Top,   alphaVertexData, alphaIndiceData, thisBlockId); }
					if(down)  { AddBlockSide(x, y, z, BlockSides::Down,  alphaVertexData, alphaIndiceData, thisBlockId); }
				}
				else
				{
					if(back)  { AddBlockSide(x, y, z, BlockSides::Back,  vertexData, indiceData, thisBlockId); }
					if(front) { AddBlockSide(x, y, z, BlockSides::Front, vertexData, indiceData, thisBlockId); }
					if(right) { AddBlockSide(x, y, z, BlockSides::Right, vertexData, indiceData, thisBlockId); }
					if(left)  { AddBlockSide(x, y, z, BlockSides::Left,  vertexData, indiceData, thisBlockId); }
					if(top)   { AddBlockSide(x, y, z, BlockSides::Top,   vertexData, indiceData, thisBlockId); }
					if(down)  { AddBlockSide(x, y, z, BlockSides::Down,  vertexData, indiceData, thisBlockId); }	
				}
			}
		}
	}

	//fill buffers
	glBindVertexArray(vaoHandle);
	
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(float), &vertexData[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indiceData.size() * sizeof(GLuint), &indiceData[0], GL_STATIC_DRAW);
	
	{
		//attribute locations
		GLint posAttrib = glGetAttribLocation(ChunkHandler::shaderProgramHandle, "position");
		glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);
		glEnableVertexAttribArray(posAttrib);

		GLint texAttrib = glGetAttribLocation(ChunkHandler::shaderProgramHandle, "texCoord");
		glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(texAttrib);
	}
	
	//and for things with alpha
	glBindVertexArray(alphaVaoHandle);
	
	glBindBuffer(GL_ARRAY_BUFFER, alphaVbo);
	glBufferData(GL_ARRAY_BUFFER, alphaVertexData.size() * sizeof(float), &alphaVertexData[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, alphaEbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, alphaIndiceData.size() * sizeof(GLuint), &alphaIndiceData[0], GL_STATIC_DRAW);
	
	{
		//attribute locations
		GLint posAttrib = glGetAttribLocation(ChunkHandler::shaderProgramHandle, "position");
		glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);
		glEnableVertexAttribArray(posAttrib);

		GLint texAttrib = glGetAttribLocation(ChunkHandler::shaderProgramHandle, "texCoord");
		glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(texAttrib);
	}

	triangleCount = indiceData.size() / 3;
	alphaTriangleCount = alphaIndiceData.size() / 3;

	glBindVertexArray(0);
}

Chunk::Chunk()
{
	glGenVertexArrays(1, &vaoHandle);
	glGenVertexArrays(1, &alphaVaoHandle);
	
	glBindVertexArray(vaoHandle);
	
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);
	
	glBindVertexArray(alphaVaoHandle);
	
	glGenBuffers(1, &alphaVbo);
	glGenBuffers(1, &alphaEbo);
	
	//RebuildVBOEBO();
}

Chunk::~Chunk()
{
	delete[] blockData;
}

void Chunk::Render()
{
	glBindVertexArray(vaoHandle);
	glDrawElements(GL_TRIANGLES, triangleCount * 3, GL_UNSIGNED_INT, 0);
	glBindVertexArray(alphaVaoHandle);
	glDrawElements(GL_TRIANGLES, alphaTriangleCount * 3, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Chunk::Update()
{

}
