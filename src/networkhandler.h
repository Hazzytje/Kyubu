#ifndef __NETWORKHANDLER_H__
#define __NETWORKHANDLER_H__
#include <thread>

class Packet;

class NetworkHandler
{
	public:
		std::thread networker;
		NetworkHandler();
		~NetworkHandler();
		bool endThread;
		bool readyToWrite;
		Packet* packet;
	private:
};
void HandleNetwork(NetworkHandler& handler);

#endif
