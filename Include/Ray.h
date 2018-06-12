#pragma once

#include "FVector.h"

class Ray
{
public:
	Ray() {}
	Ray(const FVector& InOrigin, const FVector &InDirection) { Origin = InOrigin; Direction = InDirection; }
	FVector GetOrigin() const { return Origin; }
	FVector GetDirection() const { return Direction; }
	FVector PointAtT(float T) const { return Origin + T * Direction; }

private:
	FVector Origin;
	FVector Direction;
};
