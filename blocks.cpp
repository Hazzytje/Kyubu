#include "blocks.h"
#define SETTEXTUREAT(var, x, y) (var[0] = x    , var[1] = y,     \
                                 var[2] = x + 1, var[3] = y,     \
								 var[4] = x + 1, var[5] = y + 1, \
								 var[6] = x    , var[7] = y + 1)

std::map<unsigned short, Block> Blocks::blockRegister = std::map<unsigned short, Block>();

namespace BlockTypes {
	enum Enum {
		Air, Stone, Grass, Dirt, Bedrock, Water, Lava, Sand, Gravel, GoldOre, IronOre, CoalOre, DiamondOre, Wood, Leaves, LazuliOre, SandStone, Snow, Ice, Cactus, Tree, Clay, Obsidian, Green_SugarCane, Green_Grass, Green_DeadBush, Green_FlowerYellow, Green_FlowerRed, RedstoneOre
	};
}

void Blocks::initBlocks()
{
	Block stone;
	SETTEXTUREAT(stone.topTexCoords, 1, 0);
	SETTEXTUREAT(stone.bottomTexCoords, 1, 0);
	SETTEXTUREAT(stone.northTexCoords, 1, 0);
	SETTEXTUREAT(stone.eastTexCoords, 1, 0);
	SETTEXTUREAT(stone.southTexCoords, 1, 0);
	SETTEXTUREAT(stone.westTexCoords, 1, 0);
	blockRegister[BlockTypes::Stone] = stone;

	Block grass;
	SETTEXTUREAT(grass.topTexCoords, 0, 0);
	SETTEXTUREAT(grass.bottomTexCoords, 2, 0);
	SETTEXTUREAT(grass.northTexCoords, 3, 0);
	SETTEXTUREAT(grass.eastTexCoords, 3, 0);
	SETTEXTUREAT(grass.southTexCoords, 3, 0);
	SETTEXTUREAT(grass.westTexCoords, 3, 0);
	blockRegister[BlockTypes::Grass] = grass;
	
	Block dirt;	
	SETTEXTUREAT(dirt.topTexCoords, 2, 0);
	SETTEXTUREAT(dirt.bottomTexCoords, 2, 0);
	SETTEXTUREAT(dirt.northTexCoords, 2, 0);
	SETTEXTUREAT(dirt.eastTexCoords, 2, 0);
	SETTEXTUREAT(dirt.southTexCoords, 2, 0);
	SETTEXTUREAT(dirt.westTexCoords, 2, 0);
	blockRegister[BlockTypes::Dirt] = dirt;
	
	Block bedrock;
	SETTEXTUREAT(bedrock.topTexCoords, 1, 1);
	SETTEXTUREAT(bedrock.bottomTexCoords, 1, 1);
	SETTEXTUREAT(bedrock.northTexCoords, 1, 1);
	SETTEXTUREAT(bedrock.eastTexCoords, 1, 1);
	SETTEXTUREAT(bedrock.southTexCoords, 1, 1);
	SETTEXTUREAT(bedrock.westTexCoords, 1, 1);
	blockRegister[BlockTypes::Bedrock] = bedrock;
	
	Block water;
	SETTEXTUREAT(water.topTexCoords, 15, 13);
	SETTEXTUREAT(water.bottomTexCoords, 15, 13);
	SETTEXTUREAT(water.northTexCoords, 15, 13);
	SETTEXTUREAT(water.eastTexCoords, 15, 13);
	SETTEXTUREAT(water.southTexCoords, 15, 13);
	SETTEXTUREAT(water.westTexCoords, 15, 13);
	blockRegister[BlockTypes::Water] = water;
	
	Block tree;
	SETTEXTUREAT(tree.topTexCoords, 5, 1);
	SETTEXTUREAT(tree.bottomTexCoords, 5, 1);
	SETTEXTUREAT(tree.northTexCoords, 4, 1);
	SETTEXTUREAT(tree.eastTexCoords, 4, 1);
	SETTEXTUREAT(tree.southTexCoords, 4, 1);
	SETTEXTUREAT(tree.westTexCoords, 4, 1);
	blockRegister[BlockTypes::Tree] = tree;
	
	Block leaves;
	SETTEXTUREAT(leaves.topTexCoords, 5, 3);
	SETTEXTUREAT(leaves.bottomTexCoords, 5, 3);
	SETTEXTUREAT(leaves.northTexCoords, 5, 3);
	SETTEXTUREAT(leaves.eastTexCoords, 5, 3);
	SETTEXTUREAT(leaves.southTexCoords, 5, 3);
	SETTEXTUREAT(leaves.westTexCoords, 5, 3);
	blockRegister[BlockTypes::Leaves] = leaves;
	
	Block diamondOre;
	SETTEXTUREAT(diamondOre.topTexCoords, 2, 3);
	SETTEXTUREAT(diamondOre.bottomTexCoords, 2, 3);
	SETTEXTUREAT(diamondOre.northTexCoords, 2, 3);
	SETTEXTUREAT(diamondOre.eastTexCoords, 2, 3);
	SETTEXTUREAT(diamondOre.southTexCoords, 2, 3);
	SETTEXTUREAT(diamondOre.westTexCoords, 2, 3);
	blockRegister[BlockTypes::DiamondOre] = diamondOre;
	
	Block sand;
	SETTEXTUREAT(sand.topTexCoords, 2, 1);
	SETTEXTUREAT(sand.bottomTexCoords, 2, 1);
	SETTEXTUREAT(sand.northTexCoords, 2, 1);
	SETTEXTUREAT(sand.eastTexCoords, 2, 1);
	SETTEXTUREAT(sand.southTexCoords, 2, 1);
	SETTEXTUREAT(sand.westTexCoords, 2, 1);
	blockRegister[BlockTypes::Sand] = sand;
	
	Block snow;
	SETTEXTUREAT(snow.topTexCoords, 2, 4);
	SETTEXTUREAT(snow.bottomTexCoords, 2, 4);
	SETTEXTUREAT(snow.northTexCoords, 2, 4);
	SETTEXTUREAT(snow.eastTexCoords, 2, 4);
	SETTEXTUREAT(snow.southTexCoords, 2, 4);
	SETTEXTUREAT(snow.westTexCoords, 2, 4);
	blockRegister[BlockTypes::Snow] = snow;
	
	Block ice;
	SETTEXTUREAT(ice.topTexCoords, 3, 4);
	SETTEXTUREAT(ice.bottomTexCoords, 3, 4);
	SETTEXTUREAT(ice.northTexCoords, 3, 4);
	SETTEXTUREAT(ice.eastTexCoords, 3, 4);
	SETTEXTUREAT(ice.southTexCoords, 3, 4);
	SETTEXTUREAT(ice.westTexCoords, 3, 4);
	blockRegister[BlockTypes::Ice] = ice;
}
