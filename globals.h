#ifndef __GLOBALS_H__
#define __GLOBALS_H__

#include "camera.h"
#include "game.h"

#include <iostream>

#define LELERRORS std::cout << "file: " << __FILE__ << "line: " << __LINE__; Globals::PrintAllGlErrors(); std::cout << "...\n" << std::flush;

namespace Globals
{
	extern Game* gameInstance;
	extern Game& getGameInstance();
	extern void PrintAllGlErrors();
}

#endif
