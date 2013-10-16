#include "vector3.h"
#include <math.h>

const Vector3 Vector3::Zero = Vector3();

Vector3::Vector3()
{
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
}

Vector3::Vector3(float values)
	:x(values), y(values), z(values)
{

}

Vector3::Vector3(float x, float y, float z)
	:x(x), y(y), z(z)
{
	
}

float Vector3::Length() const 
{
	return sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
}

Vector3 Vector3::Normalized() const 
{
	return (*this) / Length();
}

Vector3 Vector3::Cross(const Vector3& other) const
{
	Vector3 retVal;

	retVal.x = y * other.z - z * other.y;
	retVal.y = z * other.x - x * other.z;
	retVal.z = x * other.y - y * other.x;

	return retVal;
}

float Vector3::Dot(const Vector3& other) const
{
	return x * other.x + y * other.y + z * other.z;
}

Vector3 Vector3::operator- (const Vector3& other) const 
{
	return Vector3(x - other.x, y - other.y, z - other.z);
}

Vector3 Vector3::operator+ (const Vector3& other) const
{
	return Vector3(x + other.x, y + other.y, z + other.z);
}

Vector3 Vector3::operator* (const Vector3& other) const
{
	return Vector3(x * other.x, y * other.y, z * other.z);
}

Vector3 Vector3::operator* (const float& scale) const
{
	return Vector3(x * scale, y * scale, z * scale);
}

Vector3 Vector3::operator/ (const Vector3& other) const
{
	return Vector3(x / other.x, y / other.y, z / other.z);
}

Vector3 Vector3::operator/ (const float& scale) const
{
	return Vector3(x / scale, y / scale, z / scale);
}

Vector3& Vector3::operator-= (const Vector3& other)
{
	x = x - other.x;
	y = y - other.y;
	z = z - other.z;
	return *this;
}

Vector3& Vector3::operator+= (const Vector3& other)
{
	x = x + other.x;
	y = y + other.y;
	z = z + other.z;
	return *this;
}

Vector3& Vector3::operator*= (const Vector3& other)
{
	x = x * other.x;
	y = y * other.y;
	z = z * other.z;
	return *this;
}

Vector3& Vector3::operator*= (const float& other)
{
	x = x + other;
	y = y + other;
	z = z + other;
	return *this;
}

Vector3& Vector3::operator/= (const Vector3& other)
{
	x = x / other.x;
	y = y / other.y;
	z = z / other.z;
	return *this;
}

Vector3& Vector3::operator/= (const float& other)
{
	x = x / other;
	y = y / other;
	z = z / other;
	return *this;
}

bool Vector3::operator== (const Vector3& other) const 
{
	return x == other.x && y == other.y && z == other.z;
}

bool Vector3::operator!= (const Vector3& other) const
{
	return !operator==(other);
}