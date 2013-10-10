#include "networkhandler.h"
#include <chrono>
#include <string>
#include <string.h>
#include <cstdio>
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
 
PropertyCollection* recursiveReadProperties(Packet& pack)
{
    PropertyCollection* retVal = new PropertyCollection();
    while(true)
    {
        unsigned char valType = pack.ReadByte();
        if(valType == 0xff) return retVal;
        string keyName = pack.ReadString();
 
        switch(valType)
        {
        case PropertyTypes::Bool:
            retVal->AddBool(keyName, pack.ReadByte());
            break;
        case PropertyTypes::Int:
            retVal->AddInt(keyName, pack.ReadInt());
            break;
        case PropertyTypes::Double:
            retVal->AddDouble(keyName, pack.ReadDouble());
            printf("added %s, %f\n", keyName.c_str(), retVal->GetDouble(keyName));
            break;
        case PropertyTypes::String:
            retVal->AddString(keyName, pack.ReadString());
            break;
        case PropertyTypes::Table:
            retVal->AddTable(keyName, recursiveReadProperties(pack));
            break;
        default:
            printf("unknown enum for property type %i\n", valType);
            break;
        }
    }
 
    return retVal;
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
        		p.ReadBytes(psize);
        		p.InPos = 4;
        		
				unsigned char packetType = p.ReadByte();
				if (!p.Valid)
				{
					printf("Disconnected from server\n");
					break;
				}
				printf("Received packet 0x%02X (%d)\n", packetType, packetType);
	 
				if(packetType == Packets::Kick)
				{
					printf("Kicked from server:\"%s\"\n", p.ReadString().c_str());
				}
				else if(packetType == Packets::Login)
				{
					byte playerId = p.ReadByte();
					int entityId = p.ReadInt();
					
					Player::playerId = playerId;
					Player::entId = entityId;
				}
				else if(packetType == Packets::EntityCreate)
				{
					int entityId = p.ReadInt();
					string entityType = p.ReadString();
					PropertyCollection* kak = recursiveReadProperties(p);
					printf("health: %f\n", kak->GetTable("_ROOT_")->GetDouble("health"));
					delete kak;
				}
				else if(packetType == Packets::EntityTeleport)
				{
					int entId = p.ReadInt();
					double x = p.ReadDouble();
					double y = p.ReadDouble();
					double z = p.ReadDouble();
					
					if(entId == Player::entId)
					{
						Player::x = x;
						Player::y = y;
						Player::z = z;
					}
				}
				else if(packetType == Packets::EntityAngle)
				{
					p.ReadInt();
					p.ReadDouble();
					p.ReadDouble();
					p.ReadDouble();
				}
				else if(packetType == Packets::ChunkLoad)
				{
					short chunkX = p.ReadShort();
					short chunkY = p.ReadShort();
					
					int compressedSize = p.ReadInt();
					unsigned char* compressedChunk = p.ReadBytes(compressedSize);
					if(compressedChunk == NULL) { printf("compressedchunk == Null :/ \n"); p.Clear(); continue; }
					unsigned char* buffer = new unsigned char[compressedSize];
					memcpy(buffer, compressedChunk, compressedSize);
					
					ChunkHandler::chunkCacheMutex.lock();
					ChunkHandler::chunkCache.push(buffer);
					ChunkHandler::chunkCacheMeta.push(ChunkCache{chunkX, chunkY, compressedSize});
					ChunkHandler::chunkCacheMutex.unlock();
				}
				else if(packetType == Packets::ChangeWorld)
				{
					string kakworld = p.ReadString();
					p.ReadInt();
					printf("in world %s\n", kakworld.c_str());
				}
				else if(packetType == Packets::EntityProperty)
				{
					p.ReadInt();
					recursiveReadProperties(p);
				}
				else if(packetType == Packets::ChunkUnload)
				{
					p.ReadShort();
					p.ReadShort();
				}
				else if(packetType == Packets::PlayerJoin)
				{
					p.ReadByte();
					p.ReadString();
				}
				else if(packetType == Packets::PlayerInventoryFull)
				{
					for (int i = 0; i < 51; i++)
					{
						p.ReadShort();
						p.ReadByte();
						p.ReadByte();
					}
				}
				else
				{
					printf("Unknown packet: 0x%02X (%d)\n", packetType, packetType);
					break;
				}
				p.Clear();
        	}
            
            Player::positionMutex.lock();
            if(Player::updatedPos)
            {
            	p.WriteByte(Packets::EntityMove);
            	p.WriteDouble(Player::x);
            	p.WriteDouble(Player::y);
            	p.WriteDouble(Player::z);
            	p.Send();
            	Player::updatedPos = false;
            }
            Player::positionMutex.unlock();
        }
    }
}
 
 