#pragma once

#include "Ray.h"
#include "FVector.h"
#include "Object.h"
#include "Texture.h"

FVector RandomInUnitSphere();

bool MaterialScatter(const Material* Mat, const Ray& InRay, const FHit& Hit, FVector& Attenuation, Ray& Scattered);

class IMaterial
{
public:
	virtual bool Scatter(const Ray& InRay, const FHit& Hit, FVector& Attenuation, Ray& Scattered) const = 0;
	virtual Material* GetMaterial() = 0;
};

class Lambertian : public IMaterial
{
public:
	Lambertian(ITexture *t)
	{
		Mat.Albedo = *t->GetTexture();
		Mat.Type = EMaterialType::Lambertian;
	}

	virtual bool Scatter(const Ray& InRay, const FHit& Hit, FVector& Attenuation, Ray& Scattered) const;
	virtual Material* GetMaterial() { return &Mat; }
private:
	Material Mat;
};

class Metal : public IMaterial
{
public:
	Metal(ITexture *t, float InRoughness)
	{
		InRoughness < 1 ? Mat.Roughness = InRoughness : Mat.Roughness = 1;
		Mat.Albedo = *t->GetTexture();
		Mat.Type = EMaterialType::Metal;
	}

	virtual bool Scatter(const Ray& InRay, const FHit& Hit, FVector& Attenuation, Ray& Scattered) const;
	virtual Material* GetMaterial() { return &Mat; }
private:
	Material Mat;
};

class Dielectric : public IMaterial
{
public:
	Dielectric(float InRefIndex)
	{
		Mat.Ri = InRefIndex;
		Mat.Type = EMaterialType::Dielectric;
	}

	virtual bool Scatter(const Ray& InRay, const FHit& Hit, FVector& Attenuation, Ray& Scattered) const;
	virtual Material* GetMaterial() { return &Mat; }
private:
	Material Mat;
};
