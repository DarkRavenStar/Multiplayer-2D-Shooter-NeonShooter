#ifndef _VECTOR_3_H_
#define _VECTOR_3_H_

#include <iostream>
#include <string>
#include <cmath>
#include <math.h>

constexpr auto PI = 3.141592635;
constexpr auto M_PI = 3.141592635;
constexpr auto TWO_PI = 6.28318527;


class Vector3
{
public:
    float x,y,z;

    Vector3();
    Vector3(float X, float Y, float Z);
    void SetVector3(float X, float Y, float Z);

    ~Vector3() {};

    Vector3 operator+(const Vector3& other);
    Vector3 operator-(const Vector3& other);
    Vector3 operator*(float num);
    Vector3 operator/(float num);

    Vector3& operator+=(const Vector3& other);
    Vector3& operator-=(const Vector3& other);
    Vector3& operator*=(float val);
    Vector3& operator/=(float val);
    bool operator!=(const Vector3& other);
	bool operator==(const Vector3& other);

    float GetSquaredMagnitude();
    float GetMagnitude();
    float GetDotProduct(const Vector3& other);
    Vector3 GetCrossProduct(const Vector3& other);
    Vector3 GetNormalization();
    void Normalize();
    void SetMagnitude(float val);
    void SetLimit(float limit);
    float GetAngleBetween(Vector3 v);
	static float GetAngleBetweenTwoVector(Vector3 one, Vector3 two);
	static Vector3 Lerp(Vector3& A, Vector3& B, float t) {
		return A * t + B * (1.f - t);
	}

	static Vector3 LerpVec3(Vector3 a, Vector3 b, float t)
	{
		return Vector3
		(
			a.x + (b.x - a.x) * t,
			a.y + (b.y - a.y) * t,
			a.z + (b.z - a.z) * t
		);
	}
};

class Vector4
{
public:
    float r,g,b,a;

    Vector4()
    {
        r = 0.0f;
        g = 0.0f;
        b = 0.0f;
        a = 0.0f;
    }

    Vector4(float R, float G, float B, float A)
    {
        r = R;
        g = G;
        b = B;
        a = A;
    }

    ~Vector4() {};
};

class MatrixOffset
{
public:
	Vector3 offset[4];

	MatrixOffset()
	{
		offset[0] = Vector3(-1.0f, -1.0f, 0.0f);
		offset[1] = Vector3(-1.0f, 1.0f, 0.0f);
		offset[2] = Vector3(1.0f, 1.0f, 0.0f);
		offset[3] = Vector3(1.0f, -1.0f, 0.0f);
	}

	static MatrixOffset Center()
	{
		MatrixOffset temp;
		temp.offset[0] = Vector3(-1.0f, -1.0f, 0.0f);
		temp.offset[1] = Vector3(-1.0f, 1.0f, 0.0f);
		temp.offset[2] = Vector3(1.0f, 1.0f, 0.0f);
		temp.offset[3] = Vector3(1.0f, -1.0f, 0.0f);

		return temp;
	}

	static MatrixOffset Player()
	{
		MatrixOffset temp;
		temp.offset[0] = Vector3(-1.0f, -0.75f, 0.0f);
		temp.offset[1] = Vector3(-1.0f, 1.25f, 0.0f);
		temp.offset[2] = Vector3(1.0f, 1.25f, 0.0f);
		temp.offset[3] = Vector3(1.0f, -0.75f, 0.0f);

		return temp;
	}

	static MatrixOffset CornerUpperLeft()
	{
		MatrixOffset temp;
		temp.offset[0] = Vector3(0.0f, 0.0f, 0.0f);
		temp.offset[1] = Vector3(0.0f, 2.0f, 0.0f);
		temp.offset[2] = Vector3(2.0f, 2.0f, 0.0f);
		temp.offset[3] = Vector3(2.0f, 0.0f, 0.0f);

		return temp;
	}

	static MatrixOffset CornerLowerLeft()
	{
		MatrixOffset temp;
		temp.offset[0] = Vector3(0.0f, 0.0f, 0.0f);
		temp.offset[1] = Vector3(0.0f, -2.0f, 0.0f);
		temp.offset[2] = Vector3(2.0f, -2.0f, 0.0f);
		temp.offset[3] = Vector3(2.0f, 0.0f, 0.0f);

		return temp;
	}

	static MatrixOffset CornerUpperRight()
	{
		MatrixOffset temp;
		temp.offset[0] = Vector3(0.0f, 0.0f, 0.0f);
		temp.offset[1] = Vector3(0.0f, 2.0f, 0.0f);
		temp.offset[2] = Vector3(-2.0f, 2.0f, 0.0f);
		temp.offset[3] = Vector3(-2.0f, 0.0f, 0.0f);

		return temp;
	}
	
	static MatrixOffset CornerLowerRight()
	{
		MatrixOffset temp;
		temp.offset[0] = Vector3(0.0f, 0.0f, 0.0f);
		temp.offset[1] = Vector3(0.0f, -2.0f, 0.0f);
		temp.offset[2] = Vector3(-2.0f, -2.0f, 0.0f);
		temp.offset[3] = Vector3(-2.0f, 0.0f, 0.0f);

		return temp;
	}

	MatrixOffset(Vector3 one, Vector3 two, Vector3 three, Vector3 four)
	{
		offset[0] = one;
		offset[1] = two;
		offset[2] = three;
		offset[3] = four;
	}

	~MatrixOffset() {};
};

#endif // _VECTOR3_H_
