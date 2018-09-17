#pragma once

#include "Ray.h"
#include "FVector.h"
#include "Object.h"
#include "Texture.h"

FVector RandomInUnitSphere();

bool MaterialScatter(const ispc::Material* Mat, const Ray& InRay, const FHit& Hit, FVector& Attenuation, Ray& Scattered);

class IMaterial
{
public:
	virtual bool Scatter(const Ray& InRay, const FHit& Hit, FVector& Attenuation, Ray& Scattered) const = 0;
	virtual ispc::Material* GetMaterial() = 0;
};

class alignas(ALIGNMENT) Lambertian : public IMaterial
{
public:
	Lambertian(ITexture *t)
	{
		Mat.Albedo = *t->GetTexture();
		Mat.Type = ispc::Lambertian;
	}

	static void *operator new(size_t Bytes)
	{
		if (void* p = _aligned_malloc(Bytes, alignof(Lambertian)))
		{
			return p;
		}

		return nullptr;
	}

	static void operator delete(void *p)
	{
		_aligned_free(p);
	}

	virtual bool Scatter(const Ray& InRay, const FHit& Hit, FVector& Attenuation, Ray& Scattered) const;
	virtual ispc::Material* GetMaterial() { return &Mat; }
private:
	ispc::Material Mat;
};

class alignas(ALIGNMENT) Metal : public IMaterial
{
public:
	Metal(ITexture *t, float InRoughness)
	{
		InRoughness < 1 ? Mat.Roughness = InRoughness : Mat.Roughness = 1;
		Mat.Albedo = *t->GetTexture();
		Mat.Type = ispc::Metal;
	}

	static void *operator new(size_t Bytes)
	{
		if (void* p = _aligned_malloc(Bytes, alignof(Lambertian)))
		{
			return p;
		}

		return nullptr;
	}

	static void operator delete(void *p)
	{
		_aligned_free(p);
	}

	virtual bool Scatter(const Ray& InRay, const FHit& Hit, FVector& Attenuation, Ray& Scattered) const;
	virtual ispc::Material* GetMaterial() { return &Mat; }
private:
	ispc::Material Mat;
};

class alignas(ALIGNMENT) Dielectric : public IMaterial
{
public:
	Dielectric(float InRefIndex)
	{
		Mat.Ri = InRefIndex;
		Mat.Type = ispc::Dielectric;
	}

	static void *operator new(size_t Bytes)
	{
		if (void* p = _aligned_malloc(Bytes, alignof(Dielectric)))
		{
			return p;
		}

		return nullptr;
	}

	static void operator delete(void *p)
	{
		_aligned_free(p);
	}

	virtual bool Scatter(const Ray& InRay, const FHit& Hit, FVector& Attenuation, Ray& Scattered) const;
	virtual ispc::Material* GetMaterial() { return &Mat; }
private:
	ispc::Material Mat;
};
