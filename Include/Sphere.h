#pragma once

#include "Object.h"

class Sphere : public IObject
{
public:
	Sphere() {}
	Sphere(FVector InCenter, float InRadius, IMaterial *InMaterial) : Center(InCenter), Radius(InRadius), Material(InMaterial) {};

	virtual bool Hit(const Ray& R, float TMin, float TMax, FHit& Hit) const;
	virtual bool BoundingBox(float T0, float T1, AABB& Box) const;

private:
	FVector Center;
	float Radius;
	IMaterial *Material;
};

class MovingSphere : public IObject
{
public:
	MovingSphere() {}
	MovingSphere(FVector InCenterBegin, FVector InCenterEnd, float InBeginTime, float InEndTime, float InRadius, IMaterial *InMaterial)
		: CenterBegin(InCenterBegin), CenterEnd(InCenterEnd), BeginTime(InBeginTime), EndTime(InEndTime),
		Radius(InRadius), Material(InMaterial) {};

	virtual bool Hit(const Ray& R, float TMin, float TMax, FHit& Hit) const;
	virtual bool BoundingBox(float T0, float T1, AABB& Box) const;

	FVector Center(float Time) const;

private:
	FVector CenterBegin, CenterEnd;
	float BeginTime, EndTime;
	float Radius;
	IMaterial *Material;
};
