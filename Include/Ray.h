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

FVector Lerp(const Ray& r, const FVector &StartColor, const FVector &EndColor)
{
	FVector UnitDirection = UnitVector(r.GetDirection());
	float t = 0.5f * (UnitDirection.y + 1.0f);
	return (1.0f - t) * StartColor + t * EndColor;
}
