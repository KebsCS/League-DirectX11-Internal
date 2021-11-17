#pragma once

struct Vector4
{
	float     x, y, z, w;
	Vector4() { x = y = z = w = 0.0f; }
	Vector4(float _x, float _y, float _z, float _w) { x = _x; y = _y; z = _z; w = _w; }
};
