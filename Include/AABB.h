#pragma once

#include "FVector.h"
#include "Ray.h"

class AABB
{
public:
	AABB() {}
	AABB(const FVector& InMin, const FVector& InMax) { min = InMin; max = InMax; }

	FVector Min() const { return min; }
	FVector Max() const { return max; }

	bool Hit(const Ray& R, float TMin, float TMax) const;

private:
	FVector min;
	FVector max;
};

AABB SurroundingBox(AABB Box0, AABB Box1);
