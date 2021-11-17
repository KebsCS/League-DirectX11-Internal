#include <cmath>

#include "Vector3.h"
#include "Definitions.h"

Vector3 Vector3::Zero;
Vector3 Vector3::One(1.f, 1.f, 1.f);

Vector3::Vector3() :
	x(0.f),
	y(0.f),
	z(0.f)
{
}

Vector3::Vector3(float _x, float _y, float _z) :
	x(_x),
	y(_y),
	z(_z)
{
}

Vector3::Vector3(const Vector3& v)
{
	x = v.x;
	y = v.y;
	z = v.z;
}

Vector3& Vector3::operator=(const Vector3& v)
{
	x = v.x;
	y = v.y;
	z = v.z;

	return *this;
}

Vector3& Vector3::operator=(float f)
{
	x = f;
	y = f;
	z = f;

	return *this;
}

bool Vector3::operator==(const Vector3& v) const
{
	return (x == v.x && y == v.y && z == v.z);
}

bool Vector3::operator==(const float f) const
{
	return (x == f && y == f && z == f);
}

bool Vector3::operator!=(const Vector3& v) const
{
	return (x != v.x || y != v.y || z != v.z);
}

// + ============================================
Vector3 Vector3::operator+(const Vector3& v) const
{
	return Vector3(x + v.x, y + v.y, z + v.z);
}

Vector3 Vector3::operator+(float f) const
{
	return Vector3(x + f, y + f, z + f);
}

Vector3 Vector3::operator+=(const Vector3& v)
{
	x += v.x;
	y += v.y;
	z += v.z;

	return *this;
}

Vector3 Vector3::operator+=(float f)
{
	x += f;
	y += f;
	z += f;

	return *this;
}

// - ===================================================

Vector3 Vector3::operator-(const Vector3& v) const
{
	return Vector3(x - v.x, y - v.y, z - v.z);
}

Vector3 Vector3::operator-(float f) const
{
	return Vector3(x - f, y - f, z - f);
}

Vector3 Vector3::operator-(int) const
{
	return Vector3(-x, -y, -z);
}

Vector3 Vector3::operator-=(const Vector3& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;

	return *this;
}

Vector3 Vector3::operator-=(float f)
{
	x -= f;
	y -= f;
	z -= f;

	return *this;
}

// * ===================================================

Vector3 Vector3::operator*(const Vector3& v) const
{
	return Vector3(x * v.x, y * v.y, z * v.z);
}

Vector3 Vector3::operator*(float f) const
{
	return Vector3(x * f, y * f, z * f);
}

Vector3 Vector3::operator*=(const Vector3& v)
{
	x *= v.x;
	y *= v.y;
	z *= v.z;

	return *this;
}

Vector3 Vector3::operator*=(float f)
{
	x *= f;
	y *= f;
	z *= f;

	return *this;
}

// / ===================================================

Vector3 Vector3::operator/(const Vector3& v) const
{
	return Vector3(x / v.x, y / v.y, z / v.z);
}

Vector3 Vector3::operator/(float f) const
{
	return Vector3(x / f, y / f, z / f);
}

Vector3 Vector3::operator/=(const Vector3& v)
{
	x /= v.x;
	y /= v.y;
	z /= v.z;

	return *this;
}

Vector3 Vector3::operator/=(float f)
{
	x /= f;
	y /= f;
	z /= f;

	return *this;
}

float Vector3::Length() const
{
	return std::sqrtf((x * x) + (y * y) + (z * z));
}

float Vector3::DistTo(const Vector3& v) const
{
	Vector3 result = v - *this;
	return result.Length();
}

float Vector3::Dot(const Vector3& v) const
{
	return (x * v.x) + (y * v.y) + (z * v.z);
}

//float Vector3::Angle(const Vector3& v) const
//{
//    Vector3 v1 = *this;
//    Vector3 v2 = v;
//
//    v1.Normalize();
//    v2.Normalize();
//
//    float   Angle = v1.Dot(v2);
//
//    Angle = RAD2DEG(acosf(Angle));
//
//    return Angle;
//}

Vector3 Vector3::Cross(const Vector3& v) const
{
	return Vector3((y * v.z) - (z * v.y), (z * v.x) - (x * v.z), (x * v.y) - (y * v.x));
}

//Vector3 Vector3::TransformNormal(const Matrix& m) const
//{
//    return Vector3(XMVector3TransformNormal(Convert(), m.m));
//}
//
//Vector3 Vector3::TransformCoord(const Matrix& m) const
//{
//    return Vector3(XMVector3TransformCoord(Convert(), m.m));
//}
//
//float Vector3::Length(const Vector3& v)
//{
//    return XMVectorGetX(XMVector3Length(v.Convert()));
//}

float Vector3::Distance(const Vector3& v, const Vector3& v1)
{
	Vector3 result = v - v1;
	return result.Length();
}

//Vector3 Vector3::Normalize(const Vector3& v)
//{
//    Vector3 v1 = v;
//    v1.Normalize();
//    return v1;
//}