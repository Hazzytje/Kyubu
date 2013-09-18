#include "blocks.h"
#define SETTEXTUREAT(var, x, y) (var[0] = x    , var[1] = y,     \
                                 var[2] = x + 1, var[3] = y,     \
								 var[4] = x + 1, var[5] = y + 1, \
								 var[6] = x    , var[7] = y + 1)

std::map<unsigned short, Block> Blocks::blockRegister = std::map<unsigned short, Block>();

void Blocks::initBlocks()
{
	Block stone;
	SETTEXTUREAT(stone.topTexCoords, 1, 0);
	SETTEXTUREAT(stone.bottomTexCoords, 1, 0);
	SETTEXTUREAT(stone.northTexCoords, 1, 0);
	SETTEXTUREAT(stone.eastTexCoords, 1, 0);
	SETTEXTUREAT(stone.southTexCoords, 1, 0);
	SETTEXTUREAT(stone.westTexCoords, 1, 0);
	blockRegister[1] = stone;

	Block grass;
	SETTEXTUREAT(grass.topTexCoords, 0, 0);
	SETTEXTUREAT(grass.bottomTexCoords, 2, 0);
	SETTEXTUREAT(grass.northTexCoords, 3, 0);
	SETTEXTUREAT(grass.eastTexCoords, 3, 0);
	SETTEXTUREAT(grass.southTexCoords, 3, 0);
	SETTEXTUREAT(grass.westTexCoords, 3, 0);
	blockRegister[2] = grass;
	
	Block dirt;	
	SETTEXTUREAT(dirt.topTexCoords, 2, 0);
	SETTEXTUREAT(dirt.bottomTexCoords, 2, 0);
	SETTEXTUREAT(dirt.northTexCoords, 2, 0);
	SETTEXTUREAT(dirt.eastTexCoords, 2, 0);
	SETTEXTUREAT(dirt.southTexCoords, 2, 0);
	SETTEXTUREAT(dirt.westTexCoords, 2, 0);
	blockRegister[3] = dirt;
}
