#pragma once

#include "FVector.h"
#include"ISPCDefines.h"

FVector Sample(const ispc::Texture& T, float u, float v, const FVector& p);

class ITexture
{
public:
	virtual FVector Value(float u, float v, const FVector& p) const = 0;
	virtual ispc::Texture* GetTexture() = 0;
};

class ConstantTexture : public ITexture
{
public:
	ConstantTexture() {}
	ConstantTexture(FVector InColor)
	{
		T.Color = InColor;
		T.Type = ispc::Constant;
	}

	virtual FVector Value(float u, float v, const FVector& p) const;
	virtual ispc::Texture* GetTexture() { return &T; }

private:
	ispc::Texture T;
};

class CheckerTexture : public ITexture
{
public:
	CheckerTexture() {}
	CheckerTexture(ITexture *t0, ITexture *t1)
	{
		T.ColorEven = t0->Value(0, 0, FVector(0, 0, 0));
		T.ColorOdd = t1->Value(0, 0, FVector(0, 0, 0));
		T.Type = ispc::Checker;
	}

	virtual FVector Value(float u, float v, const FVector& p) const;
	virtual ispc::Texture* GetTexture() { return &T; }

private:
	ispc::Texture T;
};
