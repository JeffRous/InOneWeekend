#pragma once

#include "FVector.h"

class Ray
{
public:
	Ray() {}
	Ray(const FVector& InOrigin, const FVector &InDirection, float InTime) { Origin = InOrigin; Direction = InDirection; Time = InTime; }
	FVector GetOrigin() const { return Origin; }
	FVector GetDirection() const { return Direction; }
	float GetTime() const { return Time; }
	FVector PointAtT(float T) const { return Origin + T * Direction; }

private:
	FVector Origin;
	FVector Direction;
	float Time;
};
