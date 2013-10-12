#include "blocks.h"
#define SETTEXTUREAT(var, x, y) (var[0] = x    , var[1] = y,     \
                                 var[2] = x + 1, var[3] = y,     \
								 var[4] = x + 1, var[5] = y + 1, \
								 var[6] = x    , var[7] = y + 1)

std::map<unsigned short, Block> Blocks::blockRegister = std::map<unsigned short, Block>();

namespace BlockTypes {
	enum Enum {
		Air, Stone, Grass, Dirt, Bedrock, Water, Lava, Sand, Gravel, GoldOre, IronOre, CoalOre, DiamondOre, Wood, Leaves, LazuliOre, SandStone, Snow, Ice, Cactus, Tree, Clay, Obsidian, Green_SugarCane, Green_Grass, Green_DeadBush, Green_FlowerYellow, Green_FlowerRed, RedstoneOre
		//   got    got    got   got      got    got   got   got     got      got      got      got         got   got     needtex    needtex    got   got  got     got   got   got       got              needtex      needtex         got                 got              got
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
	
	Block lava;
	SETTEXTUREAT(water.topTexCoords, 15, 15);
	SETTEXTUREAT(water.bottomTexCoords, 15, 15);
	SETTEXTUREAT(water.northTexCoords, 15, 15);
	SETTEXTUREAT(water.eastTexCoords, 15, 15);
	SETTEXTUREAT(water.southTexCoords, 15, 15);
	SETTEXTUREAT(water.westTexCoords, 15, 15);
	blockRegister[BlockTypes::Lava] = lava;
	
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
	
	Block wood;
	SETTEXTUREAT(wood.topTexCoords, 4, 0);
	SETTEXTUREAT(wood.bottomTexCoords, 4, 0);
	SETTEXTUREAT(wood.northTexCoords, 4, 0);
	SETTEXTUREAT(wood.eastTexCoords, 4, 0);
	SETTEXTUREAT(wood.southTexCoords, 4, 0);
	SETTEXTUREAT(wood.westTexCoords, 4, 0);
	blockRegister[BlockTypes::Wood] = wood;
	
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
	
	Block gravel;
	SETTEXTUREAT(gravel.topTexCoords, 3, 1);
	SETTEXTUREAT(gravel.bottomTexCoords, 3, 1);
	SETTEXTUREAT(gravel.northTexCoords, 3, 1);
	SETTEXTUREAT(gravel.eastTexCoords, 3, 1);
	SETTEXTUREAT(gravel.southTexCoords, 3, 1);
	SETTEXTUREAT(gravel.westTexCoords, 3, 1);
	blockRegister[BlockTypes::Gravel] = gravel;
	
	Block goldOre;
	SETTEXTUREAT(goldOre.topTexCoords, 0, 2);
	SETTEXTUREAT(goldOre.bottomTexCoords, 0, 2);
	SETTEXTUREAT(goldOre.northTexCoords, 0, 2);
	SETTEXTUREAT(goldOre.eastTexCoords, 0, 2);
	SETTEXTUREAT(goldOre.southTexCoords, 0, 2);
	SETTEXTUREAT(goldOre.westTexCoords, 0, 2);
	blockRegister[BlockTypes::GoldOre] = goldOre;
	
	Block ironOre;
	SETTEXTUREAT(ironOre.topTexCoords, 1, 2);
	SETTEXTUREAT(ironOre.bottomTexCoords, 1, 2);
	SETTEXTUREAT(ironOre.northTexCoords, 1, 2);
	SETTEXTUREAT(ironOre.eastTexCoords, 1, 2);
	SETTEXTUREAT(ironOre.southTexCoords, 1, 2);
	SETTEXTUREAT(ironOre.westTexCoords, 1, 2);
	blockRegister[BlockTypes::IronOre] = ironOre;
	
	Block coalOre;
	SETTEXTUREAT(coalOre.topTexCoords, 2, 2);
	SETTEXTUREAT(coalOre.bottomTexCoords, 2, 2);
	SETTEXTUREAT(coalOre.northTexCoords, 2, 2);
	SETTEXTUREAT(coalOre.eastTexCoords, 2, 2);
	SETTEXTUREAT(coalOre.southTexCoords, 2, 2);
	SETTEXTUREAT(coalOre.westTexCoords, 2, 2);
	blockRegister[BlockTypes::CoalOre] = coalOre;
	
	Block redstoneOre;
	SETTEXTUREAT(redstoneOre.topTexCoords, 3, 3);
	SETTEXTUREAT(redstoneOre.bottomTexCoords, 3, 3);
	SETTEXTUREAT(redstoneOre.northTexCoords, 3, 3);
	SETTEXTUREAT(redstoneOre.eastTexCoords, 3, 3);
	SETTEXTUREAT(redstoneOre.southTexCoords, 3, 3);
	SETTEXTUREAT(redstoneOre.westTexCoords, 3, 3);
	blockRegister[BlockTypes::RedstoneOre] = redstoneOre;
	
	Block cactus;
	SETTEXTUREAT(cactus.topTexCoords, 5, 4);
	SETTEXTUREAT(cactus.bottomTexCoords, 7, 4);
	SETTEXTUREAT(cactus.northTexCoords, 6, 4);
	SETTEXTUREAT(cactus.eastTexCoords, 6, 4);
	SETTEXTUREAT(cactus.southTexCoords, 6, 4);
	SETTEXTUREAT(cactus.westTexCoords, 6, 4);
	blockRegister[BlockTypes::Cactus] = cactus;
	
	Block clay;
	SETTEXTUREAT(clay.topTexCoords, 8, 4);
	SETTEXTUREAT(clay.bottomTexCoords, 8, 4);
	SETTEXTUREAT(clay.northTexCoords, 8, 4);
	SETTEXTUREAT(clay.eastTexCoords, 8, 4);
	SETTEXTUREAT(clay.southTexCoords, 8, 4);
	SETTEXTUREAT(clay.westTexCoords, 8, 4);
	blockRegister[BlockTypes::Clay] = clay;
	
	Block obsidian;
	SETTEXTUREAT(obsidian.topTexCoords, 5, 2);
	SETTEXTUREAT(obsidian.bottomTexCoords, 5, 2);
	SETTEXTUREAT(obsidian.northTexCoords, 5, 2);
	SETTEXTUREAT(obsidian.eastTexCoords, 5, 2);
	SETTEXTUREAT(obsidian.southTexCoords, 5, 2);
	SETTEXTUREAT(obsidian.westTexCoords, 5, 2);
	blockRegister[BlockTypes::Obsidian] = obsidian;
	
	Block sugarCane;
	SETTEXTUREAT(sugarCane.topTexCoords, 9, 4);
	SETTEXTUREAT(sugarCane.bottomTexCoords, 9, 4);
	SETTEXTUREAT(sugarCane.northTexCoords, 9, 4);
	SETTEXTUREAT(sugarCane.eastTexCoords, 9, 4);
	SETTEXTUREAT(sugarCane.southTexCoords, 9, 4);
	SETTEXTUREAT(sugarCane.westTexCoords, 9, 4);
	blockRegister[BlockTypes::Green_SugarCane] = sugarCane;
	
	Block flower;
	SETTEXTUREAT(flower.topTexCoords, 13, 0);
	SETTEXTUREAT(flower.bottomTexCoords, 13, 0);
	SETTEXTUREAT(flower.northTexCoords, 13, 0);
	SETTEXTUREAT(flower.eastTexCoords, 13, 0);
	SETTEXTUREAT(flower.southTexCoords, 13, 0);
	SETTEXTUREAT(flower.westTexCoords, 13, 0);
	blockRegister[BlockTypes::Green_FlowerYellow] = flower;
	
	Block rose;
	SETTEXTUREAT(rose.topTexCoords, 12, 0);
	SETTEXTUREAT(rose.bottomTexCoords, 12, 0);
	SETTEXTUREAT(rose.northTexCoords, 12, 0);
	SETTEXTUREAT(rose.eastTexCoords, 12, 0);
	SETTEXTUREAT(rose.southTexCoords, 12, 0);
	SETTEXTUREAT(rose.westTexCoords, 12, 0);
	blockRegister[BlockTypes::Green_FlowerRed] = rose;
	
	
}
