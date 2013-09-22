#ifndef __NETWORKHANDLER_H__
#define __NETWORKHANDLER_H__
#include <thread>

class NetworkHandler
{
	public:
		std::thread networker;
		NetworkHandler();
		~NetworkHandler();
	private:
};
void HandleNetwork();

#endif // __NETWORKHANDLER_H__
