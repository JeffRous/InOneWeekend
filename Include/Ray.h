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

bool HitSphere(const FVector &Center, float Radius, const Ray& R)
{
	FVector Oc = R.GetOrigin() - Center;
	float a = Dot(R.GetDirection(), R.GetDirection());
	float b = 2.0f * Dot(Oc, R.GetDirection());
	float c = Dot(Oc, Oc) - Radius * Radius;
	float Discriminant = b * b - 4 * a * c;
	return Discriminant > 0;
}

FVector Lerp(const Ray& R, const FVector &StartColor, const FVector &EndColor)
{
	if (HitSphere(FVector(0, 0, -1), 0.5, R))
	{
		return FVector(1, 0, 0);
	}
	FVector UnitDirection = UnitVector(R.GetDirection());
	float t = 0.5f * (UnitDirection.y + 1.0f);
	return (1.0f - t) * StartColor + t * EndColor;
}
