#pragma once

#include "Object.h"
#include "Material.h"

class alignas(ALIGNMENT) Sphere : public IObject
{
public:
	Sphere() {}
	Sphere(FVector InCenter, float InRadius, IMaterial *InMaterial)
	{
		Obj.Center0 = InCenter;
		Obj.Center1 = FVector(0, 0, 0);
		Obj.Time0 = 0;
		Obj.Time1 = 0;
		Obj.Radius = InRadius;
		Obj.Type = ispc::Sphere;
		Obj.Mat = *InMaterial->GetMaterial();
	}

	static void *operator new(size_t Bytes)
	{
		if (void* p = _aligned_malloc(Bytes, alignof(Sphere)))
		{
			return p;
		}

		return nullptr;
	}

	static void operator delete(void *p)
	{
		_aligned_free(p);
	}

	virtual bool Hit(const Ray& R, float TMin, float TMax, FHit& Hit) const;
	virtual bool BoundingBox(float T0, float T1, AABB& Box) const;

	virtual ispc::Object* GetObject() { return &Obj; }

	virtual void Debug() const;

private:
	ispc::Object Obj;
};

class alignas(ALIGNMENT) MovingSphere : public IObject
{
public:
	MovingSphere() {}
	MovingSphere(FVector InCenter0, FVector InCenter1, float InTime0, float InTime1, float InRadius, IMaterial *InMaterial)
	{
		Obj.Center0 = InCenter0;
		Obj.Center1 = InCenter1;
		Obj.Time0 = InTime0;
		Obj.Time1 = InTime1;
		Obj.Radius = InRadius;
		Obj.Type = ispc::MovingSphere;
		Obj.Mat = *InMaterial->GetMaterial();
	}

	static void *operator new(size_t Bytes)
	{
		if (void* p = _aligned_malloc(Bytes, alignof(MovingSphere)))
		{
			return p;
		}

		return nullptr;
	}

	static void operator delete(void *p)
	{
		_aligned_free(p);
	}

	virtual bool Hit(const Ray& R, float TMin, float TMax, FHit& Hit) const;
	virtual bool BoundingBox(float T0, float T1, AABB& Box) const;

	virtual ispc::Object* GetObject() { return &Obj; }

	virtual void Debug() const;

	FVector GetCenterAt(float Time) const;

private:
	ispc::Object Obj;
};
