#pragma once

struct Vector4
{
	float     x, y, z, w;
	Vector4() { x = y = z = w = 0.0f; }
	Vector4(float _x, float _y, float _z, float _w) { x = _x; y = _y; z = _z; w = _w; }

	float Length()
	{
		return sqrt(x * x + y * y + z * z + w * w);
	}

	float Distance(const Vector4& o)
	{
		return (float)sqrt(pow(x - o.x, 2) + pow(y - o.y, 2) + pow(z - o.z, 2) + pow(w - o.w, 2));
	}

	Vector4 Vscale(const Vector4& s)
	{
		return Vector4(x * s.x, y * s.y, z * s.z, w * s.w);
	}

	Vector4 Scale(float s)
	{
		return Vector4(x * s, y * s, z * s, w * s);
	}

	Vector4 Normalize()
	{
		float l = Length();
		return Vector4(x / l, y / l, z / l, w / l);
	}

	Vector4 Add(const Vector4& o)
	{
		return Vector4(x + o.x, y + o.y, z + o.z, w + o.w);
	}

	Vector4 Sub(const Vector4& o)
	{
		return Vector4(x - o.x, y - o.y, z - o.z, w - o.w);
	}

	Vector4 Clone()
	{
		return Vector4(x, y, z, w);
	}
};
