#ifndef __BLOCKS_H__
#define __BLOCKS_H__

#include <map>

struct Block
{
	float topTexCoords[8];
	float bottomTexCoords[8];
	float northTexCoords[8];
	float eastTexCoords[8];
	float southTexCoords[8];
	float westTexCoords[8];
};

class Blocks
{
	public:
	static void initBlocks();
	static std::map<unsigned short, Block> blockRegister;
};

#endif
