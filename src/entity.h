#ifndef __ENTITY_H__
#define __ENTITY_H__

#include <GL/glew.h>
#include "vector3.h"
#include "boundingbox.h"

class Entity
{
	public:
		Entity();
		virtual ~Entity();
		virtual void Update();
		virtual void Draw() = 0;
		int entityId;
		Vector3 pos;
	protected:
		Vector3 vel;
		BoundingBox bbox;
	private:
		GLuint vao;
		GLuint vbo, ebo;
		int triangleCount;
		
		void MoveX(float distance);
		void MoveY(float distance);
		void MoveZ(float distance);
};

#endif