#pragma once

#include "Ray.h"

struct FHit
{
	float T; // Ray parameter
	FVector P; // 3D Point on ray
	FVector Normal;
};

class IObject
{
public:
	virtual bool Hit(const Ray& R, float TMin, float TMax, FHit& Hit) const = 0;
};

class ObjectList : public IObject
{
public:
	ObjectList() {}
	ObjectList(IObject **InList, int32 InListNumber) { List = InList; ListNumber = InListNumber; }

	virtual bool Hit(const Ray& R, float TMin, float TMax, FHit& Hit) const;

private:
	IObject **List;
	int32 ListNumber;
};
