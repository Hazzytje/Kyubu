#include "matrix.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>

Matrix::Matrix()
{
	m11 = m12 = m13 = m14 = 0.0f;
	m21 = m22 = m23 = m24 = 0.0f;
	m31 = m32 = m33 = m34 = 0.0f;
	m41 = m42 = m43 = m44 = 0.0f;
	floatValueArrayPtr = nullptr;
} 

Matrix::~Matrix()
{
	delete[] floatValueArrayPtr;
}

Matrix::Matrix(float m11, float m12, float m13, float m14, float m21, float m22, float m23, float m24, float m31, float m32, float m33, float m34, float m41, float m42, float m43, float m44)
	:m11(m11), m12(m12), m13(m13), m14(m14), m21(m21), m22(m22), m23(m23), m24(m24), m31(m31), m32(m32), m33(m33), m34(m34), m41(m41), m42(m42), m43(m43), m44(m44) 
{
	floatValueArrayPtr = nullptr;
}

Matrix::Matrix(float values)
	:m11(values), m12(values), m13(values), m14(values), m21(values), m22(values), m23(values), m24(values), m31(values), m32(values), m33(values), m34(values), m41(values), m42(values), m43(values), m44(values) 
{
	floatValueArrayPtr = nullptr;
}

Matrix Matrix::CreateRotationX(float rotation)
{
	Matrix retVal = IDENTITY_MATRIX;
	retVal.m22 = cos(rotation);
	retVal.m23 = -sin(rotation);
	retVal.m32 = sin(rotation);
	retVal.m33 = cos(rotation);
	return retVal;
}

Matrix Matrix::CreateRotationY(float rotation)
{
	Matrix retVal = IDENTITY_MATRIX;
	retVal.m11 = cos(rotation);
	retVal.m13 = sin(rotation);
	retVal.m31 = -sin(rotation);
	retVal.m33 = cos(rotation);
	return retVal;
}

Matrix Matrix::CreateRotationZ(float rotation)
{
	Matrix retVal = IDENTITY_MATRIX;
	retVal.m11 = cos(rotation);
	retVal.m12 = -sin(rotation);
	retVal.m21 = sin(rotation);
	retVal.m22 = cos(rotation);
	return retVal;
}

Matrix Matrix::CreateScale(float scale)
{
	return Matrix::CreateScale(scale, scale, scale);
}

Matrix Matrix::CreateScale(float x, float y, float z)
{
	Matrix retVal = IDENTITY_MATRIX;
	retVal.m11 = x;
	retVal.m22 = y;
	retVal.m33 = z;
	return retVal;
}

Matrix Matrix::CreateTranslation(float x, float y, float z)
{
	Matrix retVal = IDENTITY_MATRIX;
	retVal.m14 = x;
	retVal.m24 = y;
	retVal.m34 = z;
	return retVal;
}

Matrix Matrix::CreateLookAt(float eyex, float eyey, float eyez, float targetx, float targety, float targetz, float upx, float upy, float upz)
{
	return CreateLookAt(Vector3(eyex, eyey, eyez), Vector3(targetx, targety, targetz), Vector3(upx, upy, upz));
}

Matrix Matrix::CreateLookAt(Vector3 eyePos, Vector3 targetPos, Vector3 upPos)
{
	Vector3 zAxis = (eyePos - targetPos).Normalized();
	Vector3 xAxis = upPos.Cross(zAxis).Normalized();
	Vector3 yAxis = zAxis.Cross(xAxis).Normalized();
	
    Matrix orientation = Matrix(
        xAxis.x, yAxis.x, zAxis.x, 0,
        xAxis.y, yAxis.y, zAxis.y, 0,
        xAxis.z, yAxis.z, zAxis.z, 0,
          0,       0,       0,     1
    );
     
    Matrix translation = Matrix(
          1,      0,      0,     0,
          0,      1,      0,     0, 
          0,      0,      1,     0,
        -eyePos.x, -eyePos.y, -eyePos.z,  1
    );
 
    // Combine the orientation and translation to compute the view matrix
    return ( translation * orientation );
}

Matrix Matrix::CreatePerspective(float fovy, float aspect, float zNear, float zFar)
{
	float f = float(tan(M_PI_2 - 0.5f * fovy));
	float rangeInv = 1.0f / (zNear - zFar);

	Matrix retVal(0);

	retVal.m11 = f / aspect;
	retVal.m22 = f;
	retVal.m33 = (zNear + zFar) * rangeInv;
	retVal.m43 = zNear * zFar * rangeInv * 2.0f;
	retVal.m34 = -1.0;
	
	return retVal;
}

Vector3 Matrix::Translate(float x, float y, float z) const
{
	Vector3 retVal = Vector3();
	retVal.x = m11 * x + m12 * y + m13 * z + m14;
	retVal.y = m21 * x + m22 * y + m23 * z + m24;
	retVal.z = m31 * x + m32 * y + m33 * z + m34;
	return retVal;
}

Vector3 Matrix::Translate(const Vector3& input) const
{
	return Translate(input.x, input.y, input.z);
}

Matrix Matrix::operator*(const Matrix& other) const
{
	Matrix retVal;

	retVal.m11 = m11 * other.m11 + m12 * other.m21 + m13 * other.m31 + m14 * other.m41;
	retVal.m12 = m11 * other.m12 + m12 * other.m22 + m13 * other.m32 + m14 * other.m42;
	retVal.m13 = m11 * other.m13 + m12 * other.m23 + m13 * other.m33 + m14 * other.m43;
	retVal.m14 = m11 * other.m14 + m12 * other.m24 + m13 * other.m34 + m14 * other.m44;

	retVal.m21 = m21 * other.m11 + m22 * other.m21 + m23 * other.m31 + m24 * other.m41;
	retVal.m22 = m21 * other.m12 + m22 * other.m22 + m23 * other.m32 + m24 * other.m42;
	retVal.m23 = m21 * other.m13 + m22 * other.m23 + m23 * other.m33 + m24 * other.m43;
	retVal.m24 = m21 * other.m14 + m22 * other.m24 + m23 * other.m34 + m24 * other.m44;

	retVal.m31 = m31 * other.m11 + m32 * other.m21 + m33 * other.m31 + m34 * other.m41;
	retVal.m32 = m31 * other.m12 + m32 * other.m22 + m33 * other.m32 + m34 * other.m42;
	retVal.m33 = m31 * other.m13 + m32 * other.m23 + m33 * other.m33 + m34 * other.m43;
	retVal.m34 = m31 * other.m14 + m32 * other.m24 + m33 * other.m34 + m34 * other.m44;

	retVal.m41 = m41 * other.m11 + m42 * other.m21 + m43 * other.m31 + m44 * other.m41;
	retVal.m42 = m41 * other.m12 + m42 * other.m22 + m43 * other.m32 + m44 * other.m42;
	retVal.m43 = m41 * other.m13 + m42 * other.m23 + m43 * other.m33 + m44 * other.m43;
	retVal.m44 = m41 * other.m14 + m42 * other.m24 + m43 * other.m34 + m44 * other.m44;

	return retVal;
}

void Matrix::DebugPrint()
{
	printf("Matrix object at %p\n", this);
	printf("\
m11: %+f m12: %+f m13: %+f m14: %+f \n\
m21: %+f m22: %+f m23: %+f m24: %+f \n\
m31: %+f m32: %+f m33: %+f m34: %+f \n\
m41: %+f m42: %+f m43: %+f m44: %+f \n",
		 m11, m12, m13, m14,
		 m21, m22, m23, m24,
		 m31, m32, m33, m34,
		 m41, m42, m43, m44
	);	
}

float* Matrix::ToFloatArray()
{
	if(floatValueArrayPtr != nullptr)
	{
		delete[] floatValueArrayPtr;
	}
	floatValueArrayPtr = new float[16];

	floatValueArrayPtr[0 + 0] = m11;
	floatValueArrayPtr[0 + 1] = m12;
	floatValueArrayPtr[0 + 2] = m13;
	floatValueArrayPtr[0 + 3] = m14;
	
	floatValueArrayPtr[4 + 0] = m21;
	floatValueArrayPtr[4 + 1] = m22;
	floatValueArrayPtr[4 + 2] = m23;
	floatValueArrayPtr[4 + 3] = m24;
	
	floatValueArrayPtr[8 + 0] = m31;
	floatValueArrayPtr[8 + 1] = m32;
	floatValueArrayPtr[8 + 2] = m33;
	floatValueArrayPtr[8 + 3] = m34;
	
	floatValueArrayPtr[12 + 0] = m41;
	floatValueArrayPtr[12 + 1] = m42;
	floatValueArrayPtr[12 + 2] = m43;
	floatValueArrayPtr[12 + 3] = m44;

	return floatValueArrayPtr;
}
