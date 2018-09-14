#pragma once

#include "Object.h"
#include "Material.h"
#include "ISPCDefines.h"

class Sphere : public IObject
{
public:
	Sphere() {}
	Sphere(FVector InCenter, float InRadius, IMaterial *InMaterial) : Center(InCenter), Radius(InRadius), IMat(InMaterial) {};

	virtual bool Hit(const Ray& R, float TMin, float TMax, FHit& Hit) const;
	virtual bool BoundingBox(float T0, float T1, AABB& Box) const;

	virtual Object* GetObject() { return &Obj; }
	virtual EObjectType GetObjectType() const;
	virtual EMaterialType GetMaterialType() const;

	virtual void Debug() const;

private:
	FVector Center;
	float Radius;
	IMaterial *IMat;
	Object Obj;
	Material* Mat;
};

class MovingSphere : public IObject
{
public:
	MovingSphere() {}
	MovingSphere(FVector InCenter0, FVector InCenter1, float InTime0, float InTime1, float InRadius, IMaterial *InMaterial)
		: Center0(InCenter0), Center1(InCenter1), Time0(InTime0), Time1(InTime1),
		Radius(InRadius), IMat(InMaterial) {};

	virtual bool Hit(const Ray& R, float TMin, float TMax, FHit& Hit) const;
	virtual bool BoundingBox(float T0, float T1, AABB& Box) const;

	virtual Object* GetObject() { return &Obj; }
	virtual EObjectType GetObjectType() const;
	virtual EMaterialType GetMaterialType() const;

	virtual void Debug() const;

	FVector GetCenterAt(float Time) const;

private:
	FVector Center0, Center1;
	float Time0, Time1;
	float Radius;
	IMaterial *IMat;
	Object Obj;
	Material* Mat;
};
