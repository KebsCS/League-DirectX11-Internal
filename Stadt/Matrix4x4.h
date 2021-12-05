#pragma once

// todo
struct Matrix4x4
{
	float matrix[16];

	float operator [] (size_t i)
	{
		return this->matrix[i];
	}

	// todo, fix it
	//static void MultiplyMatrices(Matrix4x4& out, Matrix4x4& a, Matrix4x4& b)
	//{
	//	MultiplyMatrices(out.matrix, a.matrix, b.matrix);
	//}

	static void MultiplyMatrices(float* out, float* a, float* b)
	{
		//int size = 4 * 4;
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