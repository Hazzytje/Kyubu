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
	protected:
		Vector3 pos;
		BoundingBox bbox;
	private:
		GLuint vao;
		GLuint vbo, ebo;
		int triangleCount;
};

#endif