#pragma once

#include <algorithm>

// todo
struct Matrix4x4
{
	static const size_t size = 16;
	float matrix[size];

	float operator [] (size_t i)
	{
		return this->matrix[i];
	}

	void Transpose()
	{
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < i; j++)
			{
				std::swap(matrix[i * 4 + j], matrix[j * 4 + i]);
			}
		}
	}

	// todo, fix it
	//static void MultiplyMatrices(Matrix4x4& out, Matrix4x4& a, Matrix4x4& b)
	//{
	//	MultiplyMatrices(out.matrix, a.matrix, b.matrix);
	//}

	static void MultiplyMatrices(float* out, float* a, float* b)
	{
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				float sum = 0.f;
				for (int k = 0; k < 4; k++)
					sum = sum + a[i * 4 + k] * b[k * 4 + j];
				out[i * 4 + j] = sum;
			}
		}
	}
};