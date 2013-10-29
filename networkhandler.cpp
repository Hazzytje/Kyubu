#include "networkhandler.h"
#include <chrono>
#include <string.h>
#include "globals.h"
#include "ezSockets.h"
#include "packet.h"
#include "enums.h"
#include "config.h"

#define CHUNK_LAYER (16 * 16)
#define CHUNK_TOTALSIZE (CHUNK_LAYER * 256)

NetworkHandler::NetworkHandler()
:endThread(false), readyToWrite(false)
{
    networker = std::thread(HandleNetwork, std::ref(*this));
}
 
NetworkHandler::~NetworkHandler()
{
	endThread = true;
    networker.join();
}

void HandleNetwork(NetworkHandler& hh)
{
	NetworkHandler& handler = hh;
    ezSockets network;
 
    network.Create();
    network.mode = ezSockets::skGeneral;
    network.bBlocking = true;
    
    Config bla("default.cfg");
    
    if(network.Connect(bla.GetString("ip").c_str(), 25565))
    {
        Packet p(&network);
        printf("connection OK!\n");
 
        p.WriteByte(Packets::Login);
        p.WriteString("0.0.2");
        p.WriteString("hazzytje");
        p.WriteInt(0);
        p.Send();
        
        handler.packet = &p;
		handler.readyToWrite = true;
		
        while(!handler.endThread)
        {
        	Globals::getGameInstance().kakMutex.lock();
        	if (network.CanRead()){
        		int psize = p.ReadInt();
        	 	unsigned char* packetBuffer = p.ReadBytes(psize);
        	 	unsigned char* myBuffer = new unsigned char[psize];
        	 	memcpy(myBuffer, packetBuffer, psize);
        	 	p.Clear();
        		
        		Globals::getGameInstance().incomingPacketMutex.lock();
        		
        		Globals::getGameInstance().incomingPacketBuffer.push(std::make_pair(psize, myBuffer));
        		
        		Globals::getGameInstance().incomingPacketMutex.unlock();
        	}
			
			
        	Globals::getGameInstance().kakMutex.unlock();
			
			
        	std::this_thread::sleep_for(std::chrono::milliseconds(5));

        }
    }
}
 
 