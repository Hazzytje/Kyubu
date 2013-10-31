#ifndef __BLOCKS_H__
#define __BLOCKS_H__

#include "limits.h"

namespace BlockInfo
{
	extern float texCoordArray[SHRT_MAX * 6 * 4];
	extern short blockSettings[SHRT_MAX];
	
	inline bool GetHasAlpha(short blockId)
	{
		return blockSettings[blockId] & 1;
	}
	 
	inline bool GetHasCollision(short blockId)
	{
		return (blockSettings[blockId] & (1 << 1)) == 0;
	}
	 
	inline bool GetUseable(short blockId)
	{
		return (blockSettings[blockId] & (1 << 2)) != 0;
	}
	 
	inline bool GetDropOnDestroy(short blockId)
	{
		return (blockSettings[blockId] & (1 << 5)) == 0;
	}
	 
	inline bool GetHasCustomCollision(short blockId)
	{
		return (blockSettings[blockId] & (1 << 7)) != 0;
	}
}

#endif
