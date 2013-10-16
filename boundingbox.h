#ifndef __BOUNDINGBOX_H__
#define __BOUNDINGBOX_H__
#include "vector3.h"

class BoundingBox
{
	public:
		Vector3 Min;
		Vector3 Max;
		
		BoundingBox();
		BoundingBox(float minX, float minY, float minZ, float maxX, float maxY, float maxZ);
		BoundingBox(const Vector3& min, const Vector3& max);
		~BoundingBox();
		
		bool Contains(const BoundingBox& other);
	protected:
	private:
};

#endif
