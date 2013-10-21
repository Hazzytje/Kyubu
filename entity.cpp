#include "entity.h"

void Entity::MoveX(float distance)
{
	int direction = distance > 0.0;
	
	int inBlockX = this->pos.x;
	int inBlockY = this->pos.y;
	int inBlockZ = this->pos.z;
	
	for(int x = this->pos.x + this->bbox.Min.x; x < this->pos.x + this->bbox.Max.x + distance + 1; x++)
	{
		for(int y = this->pos.y + this->bbox.Min.y; y < this->pos.y + this->bbox.Max.y + 1; y++)
		{
			for(int z = this->pos.z + this->bbox.Min.z; z < this->pos.z + this->bbox.Max.z + 1; z++)
			{
				
			}
		}
	}
	
	BoundingBox blockBox = BoundingBox(Vector3::Zero, Vector3(1));
	
	
	
}

Entity::Entity()
{
	
}

Entity::~Entity()
{
	
}

void Entity::Update()
{
	
	
	
}