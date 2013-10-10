#ifndef __PLAYER_H__
#define __PLAYER_H__
#include <mutex>

class Player
{
	public:
		Player();
		~Player();
		
		static std::mutex positionMutex;
		static double x, y, z;
		static bool updatedPos;
		static unsigned char playerId;
		static int entId;
	private:
};

#endif // __PLAYER_H__
