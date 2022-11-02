#include <cmath>
#include <algorithm>

#include "Vector3.h"
#include "crt.h"

Vector3::Vector3()
{
	this->x = 0; this->y = 0; this->z = 0;
}

bool Vector3::operator==(Vector3 const& other) const
{
	return other.x == this->x && other.y == this->y && other.z == this->z;
}

bool Vector3::operator!=(Vector3 const& other) const
{
	return other.x == this->x && other.y == this->y && other.z == this->z;
}

bool Vector3::IsValid() const
{
	return this->x != 0/* && this->y != 0*/ && this->z != 0;
}

Vector3 Vector3::ToGround() const
{
	Vector3 result(this->x, 0, this->z);
	return result;
}

bool Vector3::IsZero(float tolerance) const
{
	return this->x > -tolerance && this->x < tolerance&&
		// this->y > -tolerance && this->y < tolerance&&
		this->z > -tolerance && this->z < tolerance;
}

float Vector3::DistanceLine(Vector3 segmentStart, Vector3 segmentEnd, bool onlyIfOnSegment, bool squared)
{
	auto objects = this->ProjectOn(segmentStart, segmentEnd);
	if (objects.IsOnSegment || onlyIfOnSegment == false)
	{
		return squared ? objects.SegmentPoint.DistanceSquared(Vector3(this->x, this->y, this->z)) : objects.SegmentPoint.Distance(Vector3(this->x, this->y, this->z));
	}
	return FLT_MAX;
}

//Vector3::operator float* ()
//{
//	return &x;
//}

float Vector3::Distance(const Vector3& to) const
{
	return (float)sqrt(pow(to.x - x, 2) + pow(to.z - z, 2) + pow(to.y - y, 2));
}

float Vector3::LengthSquared() const
{
	return this->x * this->x + this->y * this->y + this->z * this->z;
}

float Vector3::Distance(Vector3 const& segment_start, Vector3 const& segment_end, bool only_if_on_segment,
	bool squared) const
{
	auto const projection_info = this->ProjectOn(segment_start, segment_end);

	if (projection_info.IsOnSegment || !only_if_on_segment)
	{
		return squared
			? this->DistanceSquared(projection_info.SegmentPoint)
			: this->Distance(projection_info.SegmentPoint);
	}
	return FLT_MAX;
}

float Vector3::DistanceSquared(Vector3 const& to) const
{
	Vector3 delta;

	delta.x = x - to.x;
	delta.y = y - to.y;
	delta.z = z - to.z;

	return delta.LengthSquared();
}

Vector3& Vector3::operator*=(const Vector3& v)
{
	this->x *= v.x;
	this->y *= v.y;
	this->z *= v.z;
	return *this;
}

Vector3& Vector3::operator*=(float s)
{
	this->x *= s;
	this->y *= s;
	this->z *= s;
	return *this;
}

Vector3& Vector3::operator/=(const Vector3& v)
{
	this->x /= v.x;
	this->y /= v.y;
	this->z /= v.z;
	return *this;
}

Vector3& Vector3::operator/=(float s)
{
	this->x /= s;
	this->y /= s;
	this->z /= s;
	return *this;
}

Vector3& Vector3::operator+=(const Vector3& v)
{
	this->x += v.x;
	this->y += v.y;
	this->z += v.z;
	return *this;
}

Vector3& Vector3::operator+=(float fl)
{
	this->x += fl;
	this->y += fl;
	this->z += fl;
	return *this;
}

Vector3& Vector3::operator-=(const Vector3& v)
{
	this->x -= v.x;
	this->y -= v.y;
	this->z -= v.z;
	return *this;
}

Vector3& Vector3::operator-=(float fl)
{
	this->x -= fl;
	this->y -= fl;
	this->z -= fl;
	return *this;
}

Vector3 Vector3::operator-(const Vector3& v) const
{
	Vector3 result(this->x - v.x, this->y - v.y, this->z - v.z);
	return result;
}

Vector3 Vector3::operator-(float mod) const
{
	Vector3 result(this->x - mod, this->y - mod, this->z - mod);
	return result;
}

Vector3 Vector3::operator+(const Vector3& v) const
{
	Vector3 result(this->x + v.x, this->y + v.y, this->z + v.z);
	return result;
}

Vector3 Vector3::operator+(float mod) const
{
	Vector3 result(this->x + mod, this->y + mod, this->z + mod);
	return result;
}

Vector3 Vector3::operator/(const Vector3& v) const
{
	Vector3 result(this->x / v.x, this->y / v.y, this->z / v.z);
	return result;
}

Vector3 Vector3::operator/(float mod) const
{
	Vector3 result(this->x / mod, this->y / mod, this->z / mod);
	return result;
}

Vector3 Vector3::operator*(const Vector3& v) const
{
	Vector3 result(this->x * v.x, this->y * v.y, this->z * v.z);
	return result;
}

Vector3 Vector3::operator*(float mod) const
{
	Vector3 result(this->x * mod, this->y * mod, this->z * mod);
	return result;
}

Vector3& Vector3::operator=(const Vector3& v) = default;

Vector3& Vector3::SwitchYZ()
{
	const auto temp = this->y;

	this->y = this->z;
	this->z = temp;
	return *this;
}

Vector3& Vector3::Negate()
{
	this->x = -this->x;
	this->y = -this->y;
	this->z = -this->z;

	return *this;
}

float Vector3::Length() const
{
	return crt::sqrtf(this->x * this->x + this->y * this->y + this->z * this->z);
}

Vector3 Vector3::Rotate_x(float angle) const {
	return Vector3(
		this->x * cos(angle) + this->z * sin(angle),
		this->y,
		-this->x * sin(angle) + this->z * cos(angle)
	);
}

Vector3 Vector3::Rotate_z(float angle) const
{
	Vector3 result(this->x * cos(angle) - this->z * sin(angle), this->y, this->x * sin(angle) + this->z * cos(angle));
	return result;
}

Vector3 Vector3::Normalized() const
{
	auto const length = this->Length();
	if (length != 0)
	{
		auto const inv = 1.0f / length;
		return { this->x * inv, this->y * inv, this->z * inv };
	}

	return *this;
}

float Vector3::NormalizeInPlace()
{
	float radius = this->Length();

	float iradius = 1.f / (radius + FLT_EPSILON);

	x *= iradius;
	y *= iradius;
	z *= iradius;

	return radius;

	/*auto v = *this;
	auto const l = this->Length();

	if (l != 0.0f)
	{
		v /= l;
	}
	else
	{
		v.x = v.z = 0.0f; v.y = 1.0f;
	}
	return l;*/
}

float Vector3::DotProduct(Vector3 const& other) const
{
	return this->x * other.x + this->y * other.y + this->z * other.z;
}

float Vector3::CrossProduct(Vector3 const& other) const
{
	return (this->y * other.z - this->z * other.y) +
		(this->z * other.x - this->x * other.z) + (this->x * other.y - this->y * other.x);
	//	return other.y * this->x - other.x * this->y;
}

Vector3 Vector3::Center(Vector3 const& other) const
{
	return (*this + other) / 2;
}

float Vector3::Polar() const
{
	if (this->Close(x, 0.f, 0.f))
	{
		if (z > 0.f)
		{
			return 90.f;
		}
		return z < 0.f ? 270.f : 0.f;
	}

	auto theta = atan(z / x) * 180.f / M_PI;
	if (x < 0.f)
	{
		theta = theta + 180.f;
	}
	if (theta < 0.f)
	{
		theta = theta + 360.f;
	}
	return theta;
}

float Vector3::AngleBetween(Vector3 const& other) const
{
	auto theta = Polar() - other.Polar();
	if (theta < 0.f)
	{
		theta = theta + 360.f;
	}
	if (theta > 180.f)
	{
		theta = 360.f - theta;
	}
	return theta;
}

bool Vector3::Close(float a, float b, float eps) const
{
	if (abs(eps) < FLT_EPSILON)
	{
		eps = static_cast<float>(1e-9);
	}
	return abs(a - b) <= eps;
}

// Expects angle in radians!
Vector3 Vector3::Rotated(float angle) const
{
	auto const c = cos(angle);
	auto const s = sin(angle);

	return { static_cast<float>(x * c - z * s), y, static_cast<float>(z * c + x * s) };
}

Vector3 Vector3::Perpendicular() const
{
	return { -z,y,x };
}

Vector3 Vector3::Perpendicular2() const
{
	return { z,y,-x };
}

Vector3 Vector3::Extend(Vector3 const& to, float distance) const
{
	const auto from = *this;
	const auto result = from + (to - from).Normalized() * distance;
	return result;
}

Vector3 Vector3::Append(const Vector3 & pos1, const Vector3 & pos2, float dist) const
{
	return pos2 + (pos2 - pos1).Normalized() * dist;
}

ProjectionInfo::ProjectionInfo(const bool is_on_segment, Vector3 const& segment_point, Vector3 const& line_point) :
	IsOnSegment(is_on_segment), LinePoint(line_point), SegmentPoint(segment_point)
{
}

IntersectionResult::IntersectionResult(const bool intersects, Vector3 const& point) :
	Intersects(intersects), Point(point)
{
}

ProjectionInfo Vector3::ProjectOn(Vector3 const& segment_start, Vector3 const& segment_end) const
{
	float rs;
	float cx = x;
	float cz = z;
	float ax = segment_start.x;
	float az = segment_start.z;
	float bx = segment_end.x;
	float bz = segment_end.z;

	float cy = y;
	float ay = segment_start.y;
	float by = segment_end.y;

	float rl = float((cx - ax) * (bx - ax) + (cz - az) * (bz - az) + (cy - ay) * (by - ay)) / (powf(bx - ax, 2) + powf(bz - az, 2) + powf(by - ay, 2));
	Vector3 point_line = Vector3(ax + rl * (bx - ax), ay + rl * (by - ay), az + rl * (bz - az));

	//	const auto rl = ((cx - ax) * (bx - ax) + (cz - az) * (bz - az)) / (pow(bx - ax, 2) + pow(bz - az, 2));
	//	const auto point_line = Vector3(ax + rl * (bx - ax), 0, az + rl * (bz - az));

	if (rl < 0)
	{
		rs = 0;
	}
	else if (rl > 1)
	{
		rs = 1;
	}
	else
	{
		rs = rl;
	}

	bool is_on_segment = rs == rl;
	Vector3 point_segment = is_on_segment ? point_line : Vector3(ax + rs * (bx - ax), ay + rs * (by - ay), az + rs * (bz - az));
	//auto const point_segment = is_on_segment ? point_line : Vector3(ax + rs * (bx - ax), 0, az + rs * (bz - az));

	return ProjectionInfo(is_on_segment, point_segment, point_line);
}

IntersectionResult Vector3::Intersection(Vector3 const& line_segment_end, Vector3 const& line_segment2_start,
	Vector3 const& line_segment2_end) const
{
	Vector3 side1 = Vector3(line_segment_end.x - this->x, line_segment_end.y - this->y, line_segment_end.z - this->z);
	Vector3 side2 = Vector3(line_segment2_end.x - line_segment2_start.x, line_segment2_end.y - line_segment2_start.y, line_segment2_end.z - line_segment2_start.z);

	float s = (-side1.z * (this->x - line_segment2_start.x) + side1.x * (this->z - line_segment2_start.z)) / (-side2.x * side1.z + side1.x * side2.z);
	float t = (side2.x * (this->z - line_segment2_start.z) - side2.z * (this->x - line_segment2_start.x)) / (-side2.x * side1.z + side1.x * side2.z);

	if (s >= 0 && s <= 1 && t >= 0 && t <= 1)
		return { true,{ this->x + t * side1.x,  this->y + t * side1.y, this->z + t * side1.z } };

	return { false,{} };
}

Vector3 Vector3::ClampAngle()
{
	x = std::clamp(x, -89.f, 89.f);

	y = fmod(y + 180.f, 360.f) - 180.f;

	z = 0.f;

	return *this;
}