#include "boundingbox.h"

BoundingBox::BoundingBox(float minX, float minY, float minZ, float maxX, float maxY, float maxZ)
:Min(minX, minY, minZ), Max(maxX, maxY, maxZ)
{
	
}

BoundingBox::BoundingBox(const Vector3& min, const Vector3& max)
:Min(min), Max(max)
{
	
}

BoundingBox::BoundingBox()
{
	
}

BoundingBox::~BoundingBox()
{
	
}

bool BoundingBox::Contains(const BoundingBox& box)
{
	return this->Max.x > box.Min.x && 
	       this->Min.x < box.Max.x && 
	       this->Max.y > box.Min.y && 
	       this->Min.y < box.Max.y && 
	       this->Max.z > box.Min.z && 
	       this->Min.z < box.Max.z;
}