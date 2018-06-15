#pragma once

#include "Ray.h"
#include "AABB.h"

class IMaterial;

struct FHit
{
	float T; // Ray parameter
	FVector P; // 3D Point on ray
	FVector Normal;
	IMaterial *Material;
};

class IObject
{
public:
	virtual bool Hit(const Ray& R, float TMin, float TMax, FHit& Hit) const = 0;
	virtual bool BoundingBox(float T0, float T1, AABB& Box) const = 0;

	virtual void Debug() const = 0;
};

class ObjectList : public IObject
{
public:
	ObjectList() {}
	ObjectList(IObject **InList, int32 InListSize) { List = InList; ListSize = InListSize; }

	virtual bool Hit(const Ray& R, float TMin, float TMax, FHit& Hit) const;
	virtual bool BoundingBox(float T0, float T1, AABB& Box) const;

	virtual void Debug() const;

private:
	IObject **List;
	int32 ListSize;
};
