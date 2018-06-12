#pragma once

#include "Object.h"

class Sphere : public IObject
{
public:
	Sphere() {}
	Sphere(FVector InCenter, float InRadius, IMaterial *InMaterial) : Center(InCenter), Radius(InRadius), Material(InMaterial) {};

	virtual bool Hit(const Ray& R, float TMin, float TMax, FHit& Hit) const;

private:
	FVector Center;
	float Radius;
	IMaterial *Material;
};
