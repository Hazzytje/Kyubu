#include "networkhandler.h"
#include <string>
#include <string.h>
#include <cstdio>
#include <utility>
#include "globals.h"
#include "chunk.h"
#include "ezSockets.h"
#include "packets.h"
#include "player.h"
#include "packet.h"
#include "propertycollection.h"
#include "chunkHandler.h"

#define CHUNK_LAYER (16 * 16)
#define CHUNK_TOTALSIZE (CHUNK_LAYER * 256)

NetworkHandler::NetworkHandler()
{
    networker = std::thread(HandleNetwork);
}
 
NetworkHandler::~NetworkHandler()
{
    networker.detach();
}
 

 
void HandleNetwork()
{
    ezSockets network;
 
    network.Create();
    network.mode = ezSockets::skGeneral;
    network.bBlocking = true;
    if(network.Connect("home.4o3.nl", 25565))
    {
        Packet p(&network);
        printf("connection OK!\n");
 
        p.WriteByte(Packets::Login);
        p.WriteString("0.0.2");
        p.WriteString("Nigger");
        p.WriteInt(0);
        p.Send();
 
        while(true)
        {
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
        }
    }
}
 
 