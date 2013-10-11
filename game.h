#ifndef __GAME_H__
#define __GAME_H__
#include <mutex>
#include <queue>
#include <utility>
#include <GL/glew.h>
#include "chunkHandler.h"
#include "networkhandler.h"
#include "player.h"
#include "propertycollection.h"

class Game
{
	public:
		Game(GLFWwindow* windowPtr);
		~Game();
		
		std::mutex incomingPacketMutex;
		std::queue<std::pair<int, unsigned char*>> incomingPacketBuffer;
		
		std::map<int, PropertyCollection*> entityPropertyList;
		
		void Update();
		void Draw();
		
		Player& getPlayer();
		GLFWwindow* window;
	protected:
	private:
		ChunkHandler chunkHandler;
		NetworkHandler networkHandler;
		Player localPlayer;
		
};

#endif