#pragma once

#include <Windows.h>

class Vector2
{
public:
	float x, y;

	Vector2() = default;
	Vector2(float xx, float yy) : x(xx), y(yy) {}
	operator float* ();

	bool IsZero(float tolerance = 0.01f) const;

	Vector2& operator+=(const Vector2& v);
	Vector2& operator+=(float fl);
	Vector2 operator+(const Vector2& v) const;
	Vector2 operator+(float mod) const;

	Vector2& operator-=(const Vector2& v);
	Vector2& operator-=(float fl);
	Vector2 operator-(const Vector2& v) const;
	Vector2 operator-(float mod) const;

	Vector2& operator*=(const Vector2& v);
	Vector2& operator*=(float s);
	Vector2 operator*(const Vector2& v) const;
	Vector2 operator*(float mod) const;

	Vector2& operator/=(const Vector2& v);
	Vector2& operator/=(float s);
	Vector2 operator/(const Vector2& v) const;
	Vector2 operator/(float mod) const;

	[[nodiscard]] float Length() const;
	[[nodiscard]] float LengthSquared() const;
	float NormalizeInPlace();
	[[nodiscard]] float Distance(const Vector2& to) const;
	[[nodiscard]] float DistanceSquared(const Vector2& to) const;

	operator POINT(void) const
	{
		return POINT(static_cast<LONG>(this->x), static_cast<LONG>(this->y));
	}

	Vector2(POINT& p)
	{
		this->x = static_cast<float>(p.x);
		this->y = static_cast<float>(p.y);
	}
};