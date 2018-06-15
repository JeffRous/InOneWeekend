#pragma once

#include "Object.h"

class Sphere : public IObject
{
public:
	Sphere() {}
	Sphere(FVector InCenter, float InRadius, IMaterial *InMaterial) : Center(InCenter), Radius(InRadius), Material(InMaterial) {};

	virtual bool Hit(const Ray& R, float TMin, float TMax, FHit& Hit) const;
	virtual bool BoundingBox(float T0, float T1, AABB& Box) const;

	virtual void Debug() const;

private:
	FVector Center;
	float Radius;
	IMaterial *Material;
};

class MovingSphere : public IObject
{
public:
	MovingSphere() {}
	MovingSphere(FVector InCenter0, FVector InCenter1, float InTime0, float InTime1, float InRadius, IMaterial *InMaterial)
		: Center0(InCenter0), Center1(InCenter1), Time0(InTime0), Time1(InTime1),
		Radius(InRadius), Material(InMaterial) {};

	virtual bool Hit(const Ray& R, float TMin, float TMax, FHit& Hit) const;
	virtual bool BoundingBox(float T0, float T1, AABB& Box) const;

	virtual void Debug() const;

	FVector GetCenterAt(float Time) const;

private:
	FVector Center0, Center1;
	float Time0, Time1;
	float Radius;
	IMaterial *Material;
};
