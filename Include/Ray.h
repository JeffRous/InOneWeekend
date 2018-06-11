#pragma once

#include "FVector.h"

class Ray
{
public:
	Ray() {}
	Ray(const FVector& InOrigin, const FVector &InDirection) { Origin = InOrigin; Direction = InDirection; }
	FVector GetOrigin() const { return Origin; }
	FVector GetDirection() const { return Direction; }
	FVector PointAt(float t) const { return Origin + t * Direction; }

private:
	FVector Origin;
	FVector Direction;
};
