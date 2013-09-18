#ifndef __MATRIX_H__
#define __MATRIX_H__
#include "vector3.h"

#define IDENTITY_MATRIX (Matrix(1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0))

class Matrix
{
public:
	static Matrix CreateRotationX(float rotation);
	static Matrix CreateRotationY(float rotation);
	static Matrix CreateRotationZ(float rotation);
	static Matrix CreateScale(float scale);
	static Matrix CreateScale(float scaleX, float scaleY, float scaleZ);
	static Matrix CreateTranslation(float x, float y, float z);
	static Matrix CreateLookAt(float eyex, float eyey, float eyez, float targetx, float targety, float targetz, float upx, float upy, float upz);
	static Matrix CreateLookAt(Vector3 eyePos, Vector3 targetPos, Vector3 up);
	static Matrix CreatePerspective(float fovy, float aspect, float zNear, float zFar);

	~Matrix();
	Matrix();
	Matrix(float m11, float m12, float m13, float m14, float m21, float m22, float m23, float m24, float m31, float m32, float m33, float m34, float m41, float m42, float m43, float m44);
	Matrix(float values);

	void DebugPrint();

	float* ToFloatArray();

	Vector3 Translate(float x, float y, float z) const;
	Vector3 Translate(const Vector3& input) const;

	Matrix operator*(const Matrix& other) const;
//private:
	float* floatValueArrayPtr;

	float m11, m12, m13, m14;
	float m21, m22, m23, m24;
	float m31, m32, m33, m34;
	float m41, m42, m43, m44;
};

#endif
