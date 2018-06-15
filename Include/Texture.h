#pragma once

#include "FVector.h"

class ITexture
{
public:
	virtual FVector Value(float u, float v, const FVector& p) const = 0;
};

class ConstantTexture : public ITexture
{
public:
	ConstantTexture() {}
	ConstantTexture(FVector InColor) : Color(InColor) {}

	virtual FVector Value(float u, float v, const FVector& p) const;

private:
	FVector Color;
};

class CheckerTexture : public ITexture
{
public:
	CheckerTexture() {}
	CheckerTexture(ITexture *t0, ITexture *t1) : Even(t0), Odd(t1) {}

	virtual FVector Value(float u, float v, const FVector& p) const;

private:
	ITexture *Even;
	ITexture *Odd;
};
