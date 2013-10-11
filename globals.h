#ifndef __GLOBALS_H__
#define __GLOBALS_H__

#include "camera.h"
#include "game.h"
namespace Globals
{
	extern Game* gameInstance;
	extern Game& getGameInstance();
	extern void PrintAllGlErrors();
}

#endif
