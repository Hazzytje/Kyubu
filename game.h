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
#include "textrenderer.h"

class Game
{
	public:
		Game(GLFWwindow* windowPtr);
		~Game();
		
		std::mutex kakMutex;
		
		std::mutex incomingPacketMutex;
		std::queue<std::pair<int, unsigned char*>> incomingPacketBuffer;
		
		std::map<int, PropertyCollection*> entityPropertyList;
		
		void Update();
		void Draw();
		
		Player& getPlayer();
		GLFWwindow* window;
		
		ChunkHandler chunkHandler;
		NetworkHandler networkHandler;
		Player localPlayer;
		
		TextRenderer testTextRenderer;
		
		bool loggedIn;
	protected:
	private:
		
};

#endif