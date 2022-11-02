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
	operator float* () {
		return &x;
	}

	operator const float* () const {
		return &x;
	}

	bool IsValid() const;
	[[nodiscard]] Vector3 ToGround() const;
	bool operator==(const Vector3& other) const;
	bool operator!=(const Vector3& other) const;
	bool IsZero(float tolerance = 0.01f) const;

	[[nodiscard]] float DistanceLine(Vector3 segmentStart, Vector3 segmentEnd, bool onlyIfOnSegment, bool squared);
	[[nodiscard]] float Distance(const Vector3& to) const;
	[[nodiscard]] float LengthSquared() const;
	[[nodiscard]] float Distance(Vector3 const& segment_start, Vector3 const& segment_end, bool only_if_on_segment = false, bool squared = false) const;
	[[nodiscard]] float DistanceSquared(Vector3 const& to) const;

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

	[[nodiscard]] float Length() const;
	[[nodiscard]] Vector3 Rotate_x(float angle) const;
	[[nodiscard]] Vector3 Rotate_z(float angle) const;
	[[nodiscard]] Vector3 Normalized() const;
	float NormalizeInPlace();

	[[nodiscard]] float DotProduct(Vector3 const& other) const;
	[[nodiscard]] float CrossProduct(Vector3 const& other) const;
	[[nodiscard]] Vector3 Center(Vector3 const& other) const;
	[[nodiscard]] float Polar() const;
	[[nodiscard]] float AngleBetween(Vector3 const& other) const;

	bool Close(float a, float b, float eps) const;

	[[nodiscard]] Vector3 Rotated(float angle) const;
	[[nodiscard]] Vector3 Perpendicular() const;
	[[nodiscard]] Vector3 Perpendicular2() const;
	[[nodiscard]] Vector3 Extend(Vector3 const& to, float distance) const;

	[[nodiscard]] Vector3 Append(const Vector3& pos1, const Vector3& pos2, float dist) const;

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

	Vector3 ClampAngle();

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
