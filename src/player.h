#ifndef __PLAYER_H__
#define __PLAYER_H__
#include <mutex>
#include "camera.h"
#include "entity.h"

class Player : public Entity
{
	public:
		Player();
		~Player();
		
		void Update();
		void Draw();
		
		Camera camera;
		unsigned char playerId;
		
	private:
		double prevMouseX, prevMouseY;
};

#endif // __PLAYER_H__
