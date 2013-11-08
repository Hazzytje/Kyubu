#include "camera.h"
#include "matrix.h"

#include <stdio.h>
#include <GL/glfw3.h>
#define _USE_MATH_DEFINES
#include <math.h>

void Camera::UpdateLookat()
{
	lookx = x + sin(pitch) * cos(yaw);
	looky = y + sin(pitch) * sin(yaw);
	lookz = z + cos(pitch);
}

Camera::Camera()
{
	printf("initializing camera\n");
	x = 0;
	y = 4;
	z = 0;

	pitch = float(M_PI_2);
	yaw = float(M_PI_2 + M_PI);
	UpdateLookat();
}

float Camera::GetX() const { return x; }
float Camera::GetY() const { return y; }
float Camera::GetZ() const { return z; }

void Camera::SetX(float xx) { x = xx; UpdateLookat(); }
void Camera::SetY(float yy) { y = yy; UpdateLookat(); }
void Camera::SetZ(float zz) { z =  zz; UpdateLookat(); }

void Camera::SetPos(float xx, float yy, float zz) { x = xx; y = yy; z = zz; UpdateLookat(); }

void Camera::AddX(float xx) { x += xx; UpdateLookat(); }
void Camera::AddY(float yy) { y += yy; UpdateLookat(); }
void Camera::AddZ(float zz) { z +=  zz; UpdateLookat(); }

void Camera::AddPos(float xx, float yy, float zz) { x += xx; y += yy; z += zz; UpdateLookat(); }

float Camera::GetPitch() { return pitch; }
float Camera::GetYaw() { return yaw; }

void Camera::SetPitch(float pp) { pitch = pp; UpdateLookat(); }
void Camera::SetYaw(float yy) { yaw = yy; UpdateLookat(); }

void Camera::InsertViewMatrix(GLint location)
{
	Matrix lookAt = Matrix::CreateLookAt(Vector3(x, z, y), Vector3(lookx, lookz, looky), Vector3(0.0, 1.0, 0.0));

//	lookAt.DebugPrint();

	glUniformMatrix4fv(location, 1, GL_FALSE, lookAt.values);
}
