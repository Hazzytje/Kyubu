#ifndef __PLAYER_H__
#define __PLAYER_H__
#include <mutex>
#include "camera.h"

class Player
{
	public:
		Player();
		~Player();
		
		void Update();
		
		Camera camera;
		std::mutex positionMutex;
		double x, y, z;
		bool updatedPos;
		unsigned char playerId;
		int entId;
		
	private:
		double prevMouseX, prevMouseY;
};

#endif // __PLAYER_H__
