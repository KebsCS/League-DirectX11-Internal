#pragma once

#include <cmath>
#include <limits>

#ifndef M_PI
#define M_PI 3.14159265358979323846f
#define M_PI_F (float)M_PI
#endif

struct ProjectionInfo;
struct IntersectionResult;

class Vector3
{
public:
	float x, y, z;

	Vector3(float xx, float yy, float zz) : x(xx), y(yy), z(zz) {}
	Vector3();
	operator float* ();

	bool IsValid() const;
	Vector3 toGround() const;
	bool operator==(const Vector3& other) const;
	bool operator!=(const Vector3& other) const;
	bool IsZero(float tolerance = 0.01f) const;

	float DistanceLine(Vector3 segmentStart, Vector3 segmentEnd, bool onlyIfOnSegment, bool squared);

	float Distance(const Vector3& v) const;

	float distanceTo(const Vector3& v) const;
	float LengthSquared() const;
	float Distance(Vector3 const& segment_start, Vector3 const& segment_end, bool only_if_on_segment = false, bool squared = false) const;
	float DistanceSquared(Vector3 const& to) const;

	Vector3& operator*=(const Vector3& v);
	Vector3& operator*=(float s);

	Vector3& operator/=(const Vector3& v);
	Vector3& operator/=(float s);

	Vector3& operator+=(const Vector3& v);
	Vector3& operator+=(float fl);

	Vector3& operator-=(const Vector3& v);
	Vector3& operator-=(float fl);

	Vector3 operator-(const Vector3& v) const;
	Vector3 operator-(float mod) const;
	Vector3 operator+(const Vector3& v) const;
	Vector3 operator+(float mod) const;

	Vector3 operator/(const Vector3& v) const;
	Vector3 operator/(float mod) const;
	Vector3 operator*(const Vector3& v) const;
	Vector3 operator*(float mod) const;

	Vector3& operator=(const Vector3& v);

	Vector3& SwitchYZ();
	Vector3& Negate();

	float Length() const;
	Vector3 Rotate_x(float angle) const;
	Vector3 Rotate_y(float angle) const;
	Vector3 Normalized() const;
	float NormalizeInPlace() const;

	float DotProduct(Vector3 const& other) const;
	float CrossProduct(Vector3 const& other) const;
	Vector3 Center(Vector3 const& other) const;
	float Polar() const;
	float AngleBetween(Vector3 const& other) const;

	bool Close(float a, float b, float eps) const;

	Vector3 Rotated(float angle) const;
	Vector3 Perpendicular() const;
	Vector3 Perpendicular2() const;
	Vector3 Extend(Vector3 const& to, float distance) const;

	Vector3 Append(Vector3 pos1, Vector3 pos2, float dist) const;

	ProjectionInfo ProjectOn(Vector3 const& segment_start, Vector3 const& segment_end) const;
	IntersectionResult Intersection(Vector3 const& line_segment_end, Vector3 const& line_segment2_start, Vector3 const& line_segment2_end) const;

	Vector3 Scale(float s)
	{
		return Vector3(x * s, y * s, z * s);
	}

	Vector3 Rotate(Vector3 startPos, float theta)
	{
		float dx = this->x - startPos.x;
		float dz = this->z - startPos.z;

		float px = dx * cos(theta) - dz * sin(theta);
		float pz = dx * sin(theta) + dz * cos(theta);
		return { px + startPos.x, this->y, pz + startPos.z };
	}
};

struct ProjectionInfo
{
	bool IsOnSegment;
	Vector3 LinePoint;
	Vector3 SegmentPoint;

	ProjectionInfo(bool is_on_segment, Vector3 const& segment_point, Vector3 const& line_point);
};

struct IntersectionResult
{
	bool Intersects;
	Vector3 Point;

	IntersectionResult(bool intersects = false, Vector3 const& point = Vector3());
};
