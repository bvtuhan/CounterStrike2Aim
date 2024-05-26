#pragma once 
#include <math.h>
#define _USE_MATH_DEFINES
#include <cmath>

struct Vector3 {
	float x, y, z;

	Vector3 operator+(const Vector3& other) const;
	Vector3 operator-(const Vector3& other) const;
	Vector3 operator*(float scalar) const;
	Vector3 operator/(float scalar) const;

	//Default constructor
	Vector3();

	Vector3(float x, float y, float z);

	Vector3 delta(const Vector3& other) const;

	float magnitude(const Vector3&) const;
	float yawAngle(const Vector3&) const;
	float pitchAngle(const Vector3&) const;

	float normalize() const;
};