#include "game.h"
#include <iostream>
#include <string.h>
#include "packet.h"
#include "enums.h"
#include "globals.h"
#include "propertycollection.h"

Game::Game(GLFWwindow* window)
:window(window)
{
	
}

Game::~Game()
{
	
	while(incomingPacketBuffer.size() > 0)
	{
		std::cout << "Deleting pending packet with id: " << (unsigned int)incomingPacketBuffer.front().second[0] << ". \n";
		delete[] incomingPacketBuffer.front().second;
		incomingPacketBuffer.pop();
	}
}

Player& Game::getPlayer()
{
	return localPlayer;
}

void Game::Update()
{
	localPlayer.Update();
	chunkHandler.Update();
	
	if(incomingPacketBuffer.size() != 0)
	{
		Packet p;
		p.InSize = incomingPacketBuffer.front().first;
		p.InBuffer = incomingPacketBuffer.front().second;
		
		unsigned char packetType = p.ReadByte();
			
		printf("Received packet 0x%02X (%d)\n", packetType, packetType);

		if(packetType == Packets::Kick)
		{
			printf("Kicked from server:\"%s\"\n", p.ReadString().c_str());
			glfwSetWindowShouldClose(window, true);
		}
		else if(packetType == Packets::Login)
		{
			byte playerId = p.ReadByte();
			int entityId = p.ReadInt();
			
			localPlayer.playerId = playerId;
			localPlayer.entityId = entityId;
			
			localPlayer.camera.SetX(entityPropertyList[localPlayer.entityId]->GetDouble("x"));
			localPlayer.camera.SetY(entityPropertyList[localPlayer.entityId]->GetDouble("y"));
			localPlayer.camera.SetZ(entityPropertyList[localPlayer.entityId]->GetDouble("z"));
		}
		else if(packetType == Packets::EntityCreate)
		{
			int entityId = p.ReadInt();
			string entityType = p.ReadString();
			entityPropertyList[entityId] = recursiveReadProperties(p);
			printf("health: %f\n", entityPropertyList[entityId]->GetTable("_ROOT_")->GetDouble("health"));
			entityPropertyList[entityId]->AddString("_entityType_", entityType);
		}
		else if(packetType == Packets::EntityTeleport)
		{
			int entityId = p.ReadInt();
			double x = p.ReadDouble();
			double y = p.ReadDouble();
			double z = p.ReadDouble();
			
			entityPropertyList[entityId]->AddDouble("x", x);
			entityPropertyList[entityId]->AddDouble("y", y);
			entityPropertyList[entityId]->AddDouble("z", z);
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
			if(compressedChunk == NULL) { printf("compressedchunk == Null :/ \n"); p.Clear();}
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
		}
		
		delete[] incomingPacketBuffer.front().second;
		incomingPacketBuffer.pop();
	}
}

void Game::Draw()
{
	chunkHandler.Render();
}