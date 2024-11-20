#include "Vector3.h"
#include <iostream>

Vector3::Vector3() : x(0), y(0), z(0) {}

Vector3::Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

Vector3 Vector3::delta(const Vector3& other) const
{
	return Vector3(other.x - x, other.y - y, other.z - z);
}

Vector3 Vector3::operator+(const Vector3& other) const
{
	return Vector3(x + other.x, y + other.y, z + other.z);
}

Vector3 Vector3::operator-(const Vector3& other) const
{
	return Vector3(x - other.x, y - other.y, z - other.z);
}

Vector3 Vector3::operator*(float scalar) const
{
	return Vector3(x * scalar, y * scalar, z * scalar);
}

Vector3 Vector3::operator/(float scalar) const
{
	return Vector3(x / scalar, y / scalar, z / scalar);
}

float Vector3::magnitude(const Vector3& other) const
{
	return sqrtf(pow(other.x - this->x, 2) + pow(other.y - this->y, 2) + pow(other.z - this->z, 2));
}

float Vector3::yawAngle(const Vector3& other) const {
	return atan2(other.y - this->y, other.x - this->x) * (180 / M_PI);
}

float Vector3::pitchAngle(const Vector3& other) const {
	return asinf(-(other.z - this->z) / this->magnitude(other)) * (180 / M_PI);

	//or use this
	//return atan2f(-(other.z - this->z), sqrtf(pow(other.x - this->x, 2) + pow(other.y - this->y, 2))) * (180 / M_PI);

}



float Vector3::normalize() const
{
	return sqrtf(x * x + y * y + z * z);
}
