#pragma once

#include "Object.h"

class BVHNode : public IObject
{
public:
	BVHNode() {}
	BVHNode(IObject **List, int32 ListSize, float BeginTime, float EndTime);

	virtual bool Hit(const Ray& R, float TMin, float TMax, FHit& Hit) const;
	virtual bool BoundingBox(float T0, float T1, AABB& Box) const;

	virtual ispc::Object* GetObject() { return nullptr; }

	virtual void Debug() const;

private:
	IObject* Left;
	IObject* Right;
	AABB Box;
};
