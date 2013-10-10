#ifndef __GAME_H__
#define __GAME_H__
#include <mutex>
#include <vector>
#include <pair>

class Game
{
	public:
		Game();
		~Game();
		
		std::mutex incomingPacketMutex;
		std::vector<std::pair<int, unsigned char*>> incomingPacketBuffer;
	protected:
	private:
		
};

#endif
