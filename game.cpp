#include "game.h"
#include <iostream>

Game::Game()
{
	
}

Game::~Game()
{
	
	for(auto pair : incomingPacketBuffer)
	{
		std::cout << "Deleting a packet with id: " << pair.second[0] << ". \n";
		delete[] pair.second;
	}
}
