#pragma once

#include"ISPCDefines.h"

FVector Sample(const Texture& T, float u, float v, const FVector& p);

class ITexture
{
public:
	virtual FVector Value(float u, float v, const FVector& p) const = 0;
	virtual Texture* GetTexture() = 0;
};

class ConstantTexture : public ITexture
{
public:
	ConstantTexture() {}
	ConstantTexture(FVector InColor)
	{
		T.Color = InColor;
		T.Type = ETextureType::Constant;
	}

	virtual FVector Value(float u, float v, const FVector& p) const;
	virtual Texture* GetTexture() { return &T; }

private:
	Texture T;
};

class CheckerTexture : public ITexture
{
public:
	CheckerTexture() {}
	CheckerTexture(ITexture *t0, ITexture *t1)
	{
		T.ColorEven = t0->Value(0, 0, FVector(0, 0, 0));
		T.ColorOdd = t1->Value(0, 0, FVector(0, 0, 0));
		T.Type = ETextureType::Checker;
	}

	virtual FVector Value(float u, float v, const FVector& p) const;
	virtual Texture* GetTexture() { return &T; }

private:
	Texture T;
};
