#pragma once

class Vector2
{
public:
	float x, y;

	Vector2() = default;
	Vector2(float xx, float yy) : x(xx), y(yy) {}
	operator float* ();

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
};