#include <cmath>
#include "Vector2.h"
#include "crt.h"

Vector2::operator float* ()
{
	return &x;
}

bool Vector2::IsZero(float tolerance) const
{
	return this->x > -tolerance && this->x < tolerance&&
		this->y > -tolerance && this->y < tolerance;
}

Vector2& Vector2::operator+=(const Vector2& v)
{
	this->x += v.x;
	this->y += v.y;
	return *this;
}

Vector2& Vector2::operator+=(float fl)
{
	this->x += fl;
	this->y += fl;
	return *this;
}

Vector2 Vector2::operator+(const Vector2& v) const
{
	Vector2 result(this->x + v.x, this->y + v.y);
	return result;
}

Vector2 Vector2::operator+(float mod) const
{
	Vector2 result(this->x + mod, this->y + mod);
	return result;
}

Vector2& Vector2::operator-=(const Vector2& v)
{
	this->x -= v.x;
	this->y -= v.y;
	return *this;
}

Vector2& Vector2::operator-=(float fl)
{
	this->x -= fl;
	this->y -= fl;
	return *this;
}

Vector2 Vector2::operator-(const Vector2& v) const
{
	Vector2 result(this->x - v.x, this->y - v.y);
	return result;
}

Vector2 Vector2::operator-(float mod) const
{
	Vector2 result(this->x - mod, this->y - mod);
	return result;
}

Vector2& Vector2::operator*=(const Vector2& v)
{
	this->x *= v.x;
	this->y *= v.y;
	return *this;
}

Vector2& Vector2::operator*=(float s)
{
	this->x *= s;
	this->y *= s;
	return *this;
}

Vector2 Vector2::operator*(const Vector2& v) const
{
	Vector2 result(this->x * v.x, this->y * v.y);
	return result;
}

Vector2 Vector2::operator*(float mod) const
{
	Vector2 result(this->x * mod, this->y * mod);
	return result;
}

Vector2& Vector2::operator/=(const Vector2& v)
{
	this->x /= v.x;
	this->y /= v.y;
	return *this;
}

Vector2& Vector2::operator/=(float s)
{
	this->x /= s;
	this->y /= s;
	return *this;
}

Vector2 Vector2::operator/(const Vector2& v) const
{
	Vector2 result(this->x / v.x, this->y / v.y);
	return result;
}

Vector2 Vector2::operator/(float mod) const
{
	Vector2 result(this->x / mod, this->y / mod);
	return result;
}

float Vector2::Length() const
{
	return crt::sqrtf(this->x * this->x + this->y * this->y);
}

float Vector2::LengthSquared() const
{
	return this->x * this->x + this->y * this->y;
}

float Vector2::NormalizeInPlace()
{
	float l = this->Length();
	if (l != 0.0f)
	{
		x /= l;
		y /= l;
	}
	else
	{
		x = y = 0.0f;
	}
	return l;
}

float Vector2::Distance(const Vector2& to) const
{
	Vector2 delta;
	delta.x = x - to.x;
	delta.y = y - to.y;
	return delta.Length();
}

float Vector2::DistanceSquared(const Vector2& to) const
{
	Vector2 delta;
	delta.x = x - to.x;
	delta.y = y - to.y;
	return delta.LengthSquared();
}