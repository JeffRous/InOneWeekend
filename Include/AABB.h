#pragma once

#include "FVector.h"
#include "Ray.h"

class AABB
{
public:
	AABB() {}
	AABB(const FVector& InMin, const FVector& InMax) { Min = InMin; Max = InMax; }

	bool Hit(const Ray& R, float TMin, float TMax) const;

	FVector Min;
	FVector Max;
};

AABB SurroundingBox(AABB Box0, AABB Box1);
