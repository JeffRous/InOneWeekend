#include "Texture.h"

FVector Sample(const ispc::Texture& T, float u, float v, const FVector& p)
{
	if (T.Type == ispc::Constant)
	{
		return T.Color;
	}
	else if (T.Type == ispc::Checker)
	{
		float Sines = sinf(10 * p.x)*sinf(10 * p.y)*sinf(10 * p.z);

		if (Sines < 0)
		{
			return T.ColorOdd;
		}
		else
		{
			return T.ColorEven;
		}
	}
	else
	{
		return FVector(0, 0, 0);
	}
}

FVector ConstantTexture::Value(float u, float v, const FVector& p) const
{
	return Sample(T, u, v, p);
}

FVector CheckerTexture::Value(float u, float v, const FVector& p) const
{
	return Sample(T, u, v, p);
}
