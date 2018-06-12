#pragma once

#include "Object.h"

class Sphere : public IObject
{
public:
	Sphere() {}
	Sphere(FVector InCenter, float InRadius) : Center(InCenter), Radius(InRadius) {};

	virtual bool Hit(const Ray& R, float TMin, float TMax, FHit& Hit) const;

private:
	FVector Center;
	float Radius;
};
