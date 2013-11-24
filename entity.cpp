#include "entity.h"
#include "globals.h"
#include "enums.h"
#include <math.h>
#include <iostream>

void Entity::MoveX(float distance)
{
	if (distance == 0) return;
	
	Vector3 newpos = this->pos;
	newpos.x += distance;
	
	bool direction = distance > 0;
	
	BoundingBox entbox(this->bbox);
	entbox.Min += newpos;
	entbox.Max += newpos;
	
	int entheight = ceil(this->bbox.Max.z);
	
	for (int x = -ceil(this->bbox.Min.x) - 2; x < ceil(this->bbox.Max.x) + 2; x++){
		for (int y = -ceil(this->bbox.Min.y) - 2; y < ceil(this->bbox.Max.y) + 2; y++){
			for (int z = 0; z < entheight; z++){
				Vector3 blockpos = newpos + Vector3(x, y, z);
				if(Globals::getGameInstance().chunkHandler.GetBlockAt(blockpos.x, blockpos.y, blockpos.z) != BlockTypes::Air){
					BoundingBox blockbox(blockpos, blockpos + Vector3(1));
					
					if (entbox.Intersects((blockbox))){
						this->vel.x = 0;
						return;
					}
				}
			}
		}
	}
	this->pos.x += distance;
	return;
}


void Entity::MoveY(float distance)
{
	if (distance == 0) return;
	
	Vector3 newpos = this->pos;
	newpos.y += distance;
	
	bool direction = distance > 0;
	
	BoundingBox entbox(this->bbox);
	entbox.Min += newpos;
	entbox.Max += newpos;
	
	int entheight = ceil(this->bbox.Max.z);
	
	for (int x = -ceil(this->bbox.Min.x) - 2; x < ceil(this->bbox.Max.x) + 2; x++){
		for (int y = -ceil(this->bbox.Min.y) - 2; y < ceil(this->bbox.Max.y) + 2; y++){
			for (int z = 0; z < entheight; z++){
				Vector3 blockpos = newpos + Vector3(x, y, z);
				if(Globals::getGameInstance().chunkHandler.GetBlockAt(blockpos.x, blockpos.y, blockpos.z) != BlockTypes::Air){
					BoundingBox blockbox(blockpos, blockpos + Vector3(1));
					
					if (entbox.Intersects((blockbox))){
						this->vel.y = 0;
						return;
					}
				}
			}
		}
	}
	this->pos.y += distance;
	return;
}

void Entity::MoveZ(float distance)
{
	if (distance == 0) return;
	bool direction = distance > 0;
		
	while (distance != 0)
	{
		float curdo = 0;
		if (direction){
			curdo = distance < 1 ? distance : 0.2;
		}else{
			curdo = distance < -1 ? -0.2 : distance;
		}
		
		Vector3 newpos = this->pos;
		newpos.z += curdo;
		
		int zofsoidontevenknownaymore = ceil((int)newpos.z + curdo);
		if (curdo > 0)
			zofsoidontevenknownaymore += 1;
		
		BoundingBox entbox(this->bbox);
		entbox.Min += newpos;
		entbox.Max += newpos;
		
		for (int x = -ceil(this->bbox.Min.x) - 2; x < ceil(this->bbox.Max.x) + 2; x++){
			for (int y = -ceil(this->bbox.Min.y) - 2; y < ceil(this->bbox.Max.y) + 2; y++){
				Vector3 blockpos = newpos + Vector3(x, y, 0);
				blockpos.z = zofsoidontevenknownaymore;
				
				if(Globals::getGameInstance().chunkHandler.GetBlockAt(blockpos.x, blockpos.y, blockpos.z) != BlockTypes::Air){
					BoundingBox blockbox(blockpos, blockpos + Vector3(1));
					
					if (entbox.Intersects((blockbox))){
						this->vel.z = 0;
						return;
						
					}
				}
			}
		}
		
		this->pos.z += curdo;
		distance -= curdo;
	}
}

Entity::Entity()
{
	
}

Entity::~Entity()
{
	
}

void Entity::Update()
{
	if(!Globals::getGameInstance().chunkHandler.IsChunkLoadedAt(this->pos.x / 16, this->pos.y / 16)) return;
	
	this->vel.z -= 0.02f;
	
	MoveX(this->vel.x);
	MoveY(this->vel.y);
	MoveZ(this->vel.z);
}
