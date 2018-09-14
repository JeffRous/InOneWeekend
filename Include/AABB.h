#pragma once

#include "FVector.h"
#include "Ray.h"
#include "ISPCDefines.h"

class AABB
{
public:
	AABB() {}
	AABB(const FVector& InMin, const FVector& InMax) { Box.Min = InMin; Box.Max = InMax; }

	FVector Min() const { return Box.Min; }
	FVector Max() const { return Box.Max; }

	bool Hit(const Ray& R, float TMin, float TMax) const;

private:
	BoundingBox Box;
};

bool IntersectBox(const BoundingBox& B, const Ray& R, float TMin, float TMax);
BoundingBox SurroundingBox(BoundingBox Box0, BoundingBox Box1);
AABB SurroundingBox(AABB Box0, AABB Box1);
