#include "Vector2.h"

Vector2::operator float* ()
{
	return &x;
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