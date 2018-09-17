#pragma once

#include "FVector.h"
#include "Ray.h"

class alignas(ALIGNMENT) AABB
{
public:
	AABB() {}
	AABB(const FVector& InMin, const FVector& InMax) { Box.Min = InMin; Box.Max = InMax; }

	FVector Min() const { return Box.Min; }
	FVector Max() const { return Box.Max; }

	bool Hit(const Ray& R, float TMin, float TMax) const;

private:
	ispc::BoundingBox Box;
};

bool IntersectBox(const ispc::BoundingBox& B, const Ray& R, float TMin, float TMax);
ispc::BoundingBox SurroundingBox(ispc::BoundingBox Box0, ispc::BoundingBox Box1);
AABB SurroundingBox(AABB Box0, AABB Box1);
