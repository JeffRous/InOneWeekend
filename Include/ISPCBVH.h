#pragma once
#include "Object.h"
#include "AABB.h"
#include "ISPCDefines.h"

struct ISPCBVHNode
{
	void Debug() const;

	Object Obj;
	Material Mat;

	AABB Box;
	EObjectType ObjectType;
	EMaterialType MaterialType;
	IObject *Object;

	ISPCBVHNode* Parent;
	ISPCBVHNode* Left;
	ISPCBVHNode* Right;
};

struct ISPCBVH : public IObject
{
public:
	ISPCBVH() {}
	ISPCBVH(IObject **List, int32 ListSize, float BeginTime, float EndTime);

	virtual bool Hit(const Ray& R, float TMin, float TMax, FHit& Hit) const;
	virtual bool BoundingBox(float T0, float T1, AABB& Box) const;

	virtual Object* GetObject() { return nullptr; }
	virtual EObjectType GetObjectType() const;
	virtual EMaterialType GetMaterialType() const;

	virtual void Debug() const;
private:
	ISPCBVHNode *RootNode;
};
