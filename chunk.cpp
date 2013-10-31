#include <cstdlib>
#include <stdio.h>
#include <vector>
#include <GL/glew.h>
#include <GL/glfw3.h>
#include "blocks.h"
#include "globals.h"
#include "chunk.h"

#define INDEX_OF_BLOCK(x, y, z) (((z) * 256) + ((y) * 16) + (x))

#define FRONT_LEFT_TOP 	   vertexData.push_back(x);     \
                           vertexData.push_back(y);     \
                           vertexData.push_back(z + 1);

#define FRONT_LEFT_BOTTOM  vertexData.push_back(x);     \
						   vertexData.push_back(y);     \
						   vertexData.push_back(z);

#define FRONT_RIGHT_TOP    vertexData.push_back(x + 1); \
                           vertexData.push_back(y);     \
                           vertexData.push_back(z + 1);

#define FRONT_RIGHT_BOTTOM vertexData.push_back(x + 1); \
                           vertexData.push_back(y);     \
                           vertexData.push_back(z);

#define BACK_LEFT_TOP      vertexData.push_back(x);     \
                           vertexData.push_back(y + 1); \
                           vertexData.push_back(z + 1);

#define BACK_LEFT_BOTTOM   vertexData.push_back(x);     \
                           vertexData.push_back(y + 1); \
                           vertexData.push_back(z);

#define BACK_RIGHT_TOP     vertexData.push_back(x + 1); \
                           vertexData.push_back(y + 1); \
                           vertexData.push_back(z + 1);

#define BACK_RIGHT_BOTTOM  vertexData.push_back(x + 1); \
                           vertexData.push_back(y + 1); \
                           vertexData.push_back(z);
void AddIndices(unsigned int vertexCount, std::vector<GLuint>& indiceData)
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

void Chunk::RebuildVBOEBO()
{
	
	auto vertexData = std::vector<float>();
	vertexData.reserve(50000);
	auto indiceData = std::vector<GLuint>();
	indiceData.reserve(10000);

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
				bool north = false, east = false, south = false, west = false, top = false, bottom = false;
				//south side of block
				if(x != 0)
				{
					unsigned short southBlockType = blockData[INDEX_OF_BLOCK(x - 1, y, z)];
					if(BlockInfo::GetHasAlpha(thisBlockId))
					{
						south = thisBlockId != southBlockType || BlockInfo::GetHasCustomCollision(thisBlockId);
					}
					else
					{
						if(southBlockType == 0 || BlockInfo::GetHasAlpha(southBlockType))
						{
							south = true;
						}
					}
				} // TODO: else check other chunks
				else { south = true; }
				//north side of block
				if(x != 15)
				{
					unsigned short northBlockType = blockData[INDEX_OF_BLOCK(x + 1, y, z)];
					if(BlockInfo::GetHasAlpha(thisBlockId))
					{
						north = thisBlockId != northBlockType || BlockInfo::GetHasCustomCollision(thisBlockId);
					}
					else
					{
						if(northBlockType == 0 || BlockInfo::GetHasAlpha(northBlockType))
						{
							north = true;
						}
					}
				}
				else { north = true; }
				//west side of block
				if(y != 15)
				{
					unsigned short westBlockType = blockData[INDEX_OF_BLOCK(x, y + 1, z)];
					if(BlockInfo::GetHasAlpha(thisBlockId))
					{
						west = thisBlockId != westBlockType || BlockInfo::GetHasCustomCollision(thisBlockId);
					}
					else
					{
						if(westBlockType == 0 || BlockInfo::GetHasAlpha(westBlockType))
						{
							west = true;
						}
					}
				}
				else { west = true; }
				//east side of block
				if(y != 0)
				{
					unsigned short eastBlockType = blockData[INDEX_OF_BLOCK(x, y - 1, z)];
					if(BlockInfo::GetHasAlpha(thisBlockId))
					{
						east = thisBlockId != eastBlockType || BlockInfo::GetHasCustomCollision(thisBlockId);
					}
					else
					{
						if(eastBlockType == 0 || BlockInfo::GetHasAlpha(eastBlockType))
						{
							east = true;
						}
					}
				}
				else { east = true; }
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
				//bottom side of block
				if(z != 0)
				{
					unsigned short botBlockType = blockData[INDEX_OF_BLOCK(x, y, z - 1)];
					if(BlockInfo::GetHasAlpha(thisBlockId))
					{
						bottom = thisBlockId != botBlockType || BlockInfo::GetHasCustomCollision(thisBlockId);
					}
					else
					{
						if(botBlockType == 0 || BlockInfo::GetHasAlpha(botBlockType))
						{
							bottom = true;
						}
					}
				}
				else {bottom = true;}

				float* thisBlock = BlockInfo::texCoordArray + thisBlockId * 6 * 4;

				if(south) // back
				{
					BACK_LEFT_TOP;
					vertexData.push_back(thisBlock[0 + 5 * 4]);
					vertexData.push_back(thisBlock[1 + 5 * 4]);
					FRONT_LEFT_TOP;
					vertexData.push_back(thisBlock[2 + 5 * 4] + thisBlock[0 + 5 * 4]);
					vertexData.push_back(thisBlock[1 + 5 * 4]);
					FRONT_LEFT_BOTTOM;
					vertexData.push_back(thisBlock[2 + 5 * 4] + thisBlock[0 + 5 * 4]);
					vertexData.push_back(thisBlock[3 + 5 * 4] + thisBlock[1 + 5 * 4]);
					BACK_LEFT_BOTTOM;
					vertexData.push_back(thisBlock[0 + 5 * 4]);
					vertexData.push_back(thisBlock[3 + 5 * 4] + thisBlock[1 + 5 * 4]);
					unsigned int vertexCount = vertexData.size() / 5;
					AddIndices(vertexCount, indiceData);
				}

				if(north) // front
				{
					FRONT_RIGHT_TOP;
					vertexData.push_back(thisBlock[0 + 4 * 4]);
					vertexData.push_back(thisBlock[1 + 4 * 4]);
					BACK_RIGHT_TOP
					vertexData.push_back(thisBlock[2 + 4 * 4] + thisBlock[0 + 4 * 4]);
					vertexData.push_back(thisBlock[1 + 4 * 4]);
					BACK_RIGHT_BOTTOM;
					vertexData.push_back(thisBlock[2 + 4 * 4] + thisBlock[0 + 4 * 4]);
					vertexData.push_back(thisBlock[3 + 4 * 4] + thisBlock[1 + 4 * 4]);
					FRONT_RIGHT_BOTTOM;
					vertexData.push_back(thisBlock[0 + 4 * 4]);
					vertexData.push_back(thisBlock[3 + 4 * 4] + thisBlock[1 + 4 * 4]);
					unsigned int vertexCount = vertexData.size() / 5;
					AddIndices(vertexCount, indiceData);
				}

				if(east) // right
				{
					FRONT_LEFT_TOP;
					vertexData.push_back(thisBlock[0 + 3 * 4]);
					vertexData.push_back(thisBlock[1 + 3 * 4]);
					FRONT_RIGHT_TOP;
					vertexData.push_back(thisBlock[2 + 3 * 4] + thisBlock[0 + 3 * 4]);
					vertexData.push_back(thisBlock[1 + 3 * 4]);
					FRONT_RIGHT_BOTTOM;
					vertexData.push_back(thisBlock[2 + 3 * 4] + thisBlock[0 + 3 * 4]);
					vertexData.push_back(thisBlock[3 + 3 * 4] + thisBlock[1 + 3 * 4]);
					FRONT_LEFT_BOTTOM;
					vertexData.push_back(thisBlock[0 + 3 * 4]);
					vertexData.push_back(thisBlock[3 + 3 * 4] + thisBlock[1 + 3 * 4]);
					unsigned int vertexCount = vertexData.size() / 5;
					AddIndices(vertexCount, indiceData);
				}

				if(west) // left
				{
					BACK_RIGHT_TOP;
					vertexData.push_back(thisBlock[0 + 2 * 4]);
					vertexData.push_back(thisBlock[1 + 2 * 4]);
					BACK_LEFT_TOP;
					vertexData.push_back(thisBlock[2 + 2 * 4] + thisBlock[0 + 2 * 4]);
					vertexData.push_back(thisBlock[1 + 2 * 4]);
					BACK_LEFT_BOTTOM;
					vertexData.push_back(thisBlock[2 + 2 * 4] + thisBlock[0 + 2 * 4]);
					vertexData.push_back(thisBlock[3 + 2 * 4] + thisBlock[1 + 2 * 4]);
					BACK_RIGHT_BOTTOM;
					vertexData.push_back(thisBlock[0 + 2 * 4]);
					vertexData.push_back(thisBlock[3 + 2 * 4] + thisBlock[1 + 2 * 4]);
					unsigned int vertexCount = vertexData.size() / 5;
					AddIndices(vertexCount, indiceData);
				}

				if(top)
				{
					BACK_LEFT_TOP;
					vertexData.push_back(thisBlock[0]);
					vertexData.push_back(thisBlock[1]);
					BACK_RIGHT_TOP;
					vertexData.push_back(thisBlock[2] + thisBlock[0]);
					vertexData.push_back(thisBlock[1]);
					FRONT_RIGHT_TOP;
					vertexData.push_back(thisBlock[2] + thisBlock[0]);
					vertexData.push_back(thisBlock[3] + thisBlock[1]);
					FRONT_LEFT_TOP;
					vertexData.push_back(thisBlock[0]);
					vertexData.push_back(thisBlock[3] + thisBlock[1]);
					unsigned int vertexCount = vertexData.size() / 5;
					AddIndices(vertexCount, indiceData);
				}

				if(bottom) // down
				{
					FRONT_LEFT_BOTTOM;
					vertexData.push_back(thisBlock[0 + 1 * 4]);
					vertexData.push_back(thisBlock[1 + 1 * 4]);
					FRONT_RIGHT_BOTTOM;
					vertexData.push_back(thisBlock[2 + 1 * 4] + thisBlock[0 + 1 * 4]);
					vertexData.push_back(thisBlock[1 + 1 * 4]);
					BACK_RIGHT_BOTTOM;
					vertexData.push_back(thisBlock[2 + 1 * 4] + thisBlock[0 + 1 * 4]);
					vertexData.push_back(thisBlock[3 + 1 * 4] + thisBlock[1 + 1 * 4]);
					BACK_LEFT_BOTTOM;
					vertexData.push_back(thisBlock[0 + 1 * 4]);
					vertexData.push_back(thisBlock[3 + 1 * 4] + thisBlock[1 + 1 * 4]);
					unsigned int vertexCount = vertexData.size() / 5;
					AddIndices(vertexCount, indiceData);
				}
			}
		}
	}

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(float), &vertexData[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indiceData.size() * sizeof(GLuint), &indiceData[0], GL_STATIC_DRAW);

	//attribute locations
	GLint posAttrib = glGetAttribLocation(ChunkHandler::shaderProgramHandle, "position");
	glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);
	glEnableVertexAttribArray(posAttrib);

	GLint texAttrib = glGetAttribLocation(ChunkHandler::shaderProgramHandle, "texCoord");
	glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(texAttrib);

	triangleCount = indiceData.size() / 3;

	glBindVertexArray(0);
}

Chunk::Chunk()
{
	glGenVertexArrays(1, &vaoHandle);
	glBindVertexArray(vaoHandle);
	
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);
	
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
	glBindVertexArray(0);
}

void Chunk::Update()
{

}
