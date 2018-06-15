#include "Texture.h"

FVector ConstantTexture::Value(float u, float v, const FVector& p) const
{
	return Color;
}

FVector CheckerTexture::Value(float u, float v, const FVector& p) const
{
	float Sines = sinf(10 * p.x)*sinf(10 * p.y)*sinf(10 * p.z);

	if (Sines < 0)
	{
		return Odd->Value(u, v, p);
	}
	else
	{
		return Even->Value(u, v, p);
	}
}
