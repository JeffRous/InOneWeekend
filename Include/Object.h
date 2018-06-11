#pragma once

#include "Ray.h"

struct FHit
{
	float Time;
	FVector Point;
	FVector Normal;
};

class IObject
{
public:
	virtual bool Hit(const Ray& R, float TMin, float TMax, FHit& Hit) = 0;
};
