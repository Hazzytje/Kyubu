#ifndef __VECTOR3_H__
#define __VECTOR3_H__

class Vector3
{
public:
	float x, y, z;
	Vector3();
	Vector3(float values);
	Vector3(float x, float y, float z);

	float Length() const;
	Vector3 Normalized() const;

	Vector3 Cross(const Vector3& other) const;
	float Dot(const Vector3& other) const;

	Vector3 operator- (const Vector3& other) const;
	Vector3 operator+ (const Vector3& other) const;
	Vector3 operator* (const Vector3& other) const;
	Vector3 operator* (const float& other) const;
	Vector3 operator/ (const Vector3& other) const;
	Vector3 operator/ (const float& other) const;
	
	Vector3& operator-= (const Vector3& other);
	Vector3& operator+= (const Vector3& other);
	Vector3& operator*= (const Vector3& other);
	Vector3& operator*= (const float& other);
	Vector3& operator/= (const Vector3& other);
	Vector3& operator/= (const float& other);

	bool operator== (const Vector3& other) const;
	bool operator!= (const Vector3& other) const;
	
	const static Vector3 Zero;
};

#endif
