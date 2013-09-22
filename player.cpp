 #include "player.h"

Player::Player()
{
	//ctor
}

Player::~Player()
{
	//dtor
}

std::mutex Player::positionMutex;
double Player::x, Player::y, Player::z;
bool Player::updatedPos = false;