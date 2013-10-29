#include "game.h"
#include <iostream>
#include <string.h>
#include <zlib.h>
#include <GL/glew.h>
#include <GL/glfw3.h>
#include "packet.h"
#include "enums.h"
#include "globals.h"
#include "propertycollection.h"

Game::Game(GLFWwindow* window)
:window(window), loggedIn(false)
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
	Vector3 oldPos = localPlayer.pos;
	localPlayer.Update();
	if(networkHandler.readyToWrite && loggedIn)
	{
		kakMutex.lock();
		
		Packet& packetSender = *networkHandler.packet;
		packetSender.WriteByte(Packets::EntityTeleport);
		packetSender.WriteDouble(localPlayer.pos.x);
		packetSender.WriteDouble(localPlayer.pos.y);
		packetSender.WriteDouble(localPlayer.pos.z);
		packetSender.Send();
		
		packetSender.WriteByte(Packets::EntityAngle);
		packetSender.WriteDouble(localPlayer.camera.GetPitch());
		packetSender.WriteDouble(localPlayer.camera.GetYaw());
		packetSender.WriteDouble(0.0f);
		packetSender.Send();
		
		if(glfwGetKey(window, GLFW_KEY_W))
		{
			packetSender.WriteByte(Packets::PlayerKey);
			packetSender.WriteByte(GLFW_KEY_W);
			packetSender.WriteByte(true);
			packetSender.Send();
		}
		else
		{
			packetSender.WriteByte(Packets::PlayerKey);
			packetSender.WriteByte(GLFW_KEY_W);
			packetSender.WriteByte(false);
			packetSender.Send();
		}
		
		if(glfwGetKey(window, GLFW_KEY_A))
		{
			packetSender.WriteByte(Packets::PlayerKey);
			packetSender.WriteByte(GLFW_KEY_A);
			packetSender.WriteByte(true);
			packetSender.Send();
		}
		else
		{
			packetSender.WriteByte(Packets::PlayerKey);
			packetSender.WriteByte(GLFW_KEY_A);
			packetSender.WriteByte(false);
			packetSender.Send();
		}
		
		if(glfwGetKey(window, GLFW_KEY_S))
		{
			packetSender.WriteByte(Packets::PlayerKey);
			packetSender.WriteByte(GLFW_KEY_S);
			packetSender.WriteByte(true);
			packetSender.Send();
		}
		else
		{
			packetSender.WriteByte(Packets::PlayerKey);
			packetSender.WriteByte(GLFW_KEY_S);
			packetSender.WriteByte(false);
			packetSender.Send();
		}
		
		if(glfwGetKey(window, GLFW_KEY_D))
		{
			packetSender.WriteByte(Packets::PlayerKey);
			packetSender.WriteByte(GLFW_KEY_D);
			packetSender.WriteByte(true);
			packetSender.Send();
		}
		else
		{
			packetSender.WriteByte(Packets::PlayerKey);
			packetSender.WriteByte(GLFW_KEY_D);
			packetSender.WriteByte(false);
			packetSender.Send();
		}
		
		kakMutex.unlock();
	}
	chunkHandler.Update();
	
	if(incomingPacketBuffer.size() != 0)
	{
		Packet p;
		p.InSize = incomingPacketBuffer.front().first;
		p.InBuffer = incomingPacketBuffer.front().second;
		
		unsigned char packetType = p.ReadByte();
			
		//printf("Received packet 0x%02X (%d)\n", packetType, packetType);

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
			
			localPlayer.pos.x = entityPropertyList[localPlayer.entityId]->GetDouble("x");
			localPlayer.pos.y = entityPropertyList[localPlayer.entityId]->GetDouble("y");
			localPlayer.pos.z = entityPropertyList[localPlayer.entityId]->GetDouble("z");
			
			loggedIn = true;
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
		else if (packetType == Packets::ServerData)
		{
			int itemCount = p.ReadInt();
			int itemTextureWidth = p.ReadInt();
			int itemTextureHeight = p.ReadInt();
			
			int itemTextureZlibBufferSize = p.ReadInt();
			byte* itemTextureCompressed = p.ReadBytes(itemTextureZlibBufferSize);
			int itemCoordZlibBufferSize = p.ReadInt();
			byte* itemCoordCompressed = p.ReadBytes(itemCoordZlibBufferSize);
			
			int blockCount = p.ReadInt();
			int blockTextureWidth = p.ReadInt();
			int blockTextureHeight = p.ReadInt();
			
			int blockTextureZlibBufferSize = p.ReadInt();
			byte* blockTextureCompressed = p.ReadBytes(blockTextureZlibBufferSize);
			int blockCoordZlibBufferSize = p.ReadInt();
			byte* blockCoordCompressed = p.ReadBytes(blockCoordZlibBufferSize);
			
			int blockSettingsZlibBufferSize = p.ReadInt();
			byte* blockSettingsCompressed = p.ReadBytes(blockSettingsZlibBufferSize);
			
			int blockBoxZlibBufferSize = p.ReadInt();
			byte* blockBoxCompressed = p.ReadBytes(blockBoxZlibBufferSize);
			
			int recipeCount = p.ReadInt();
			
			int recipeZlibBufferSize = p.ReadInt();
			byte* recipesCompressed = p.ReadBytes(recipeZlibBufferSize);
			
			uLongf outsize = blockTextureWidth * blockTextureHeight * 4;
			byte texture[blockTextureWidth * blockTextureHeight * 4];
			
			uncompress(texture, &outsize, blockTextureCompressed, blockTextureZlibBufferSize);
			
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, chunkHandler.textureHandle);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, blockTextureWidth, blockTextureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture);
			
			
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