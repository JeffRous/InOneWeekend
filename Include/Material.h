#pragma once

#include "Ray.h"
#include "FVector.h"
#include "Object.h"
#include "Texture.h"

FVector RandomInUnitSphere();

class IMaterial
{
public:
	virtual bool Scatter(const Ray& InRay, const FHit& Hit, FVector& Attenuation, Ray& Scattered) const = 0;
	virtual EMaterialType GetMaterialType() const = 0;
};

class Lambertian : public IMaterial
{
public:
	Lambertian(ITexture *t) : Albedo(t) {}

	virtual bool Scatter(const Ray& InRay, const FHit& Hit, FVector& Attenuation, Ray& Scattered) const;
	virtual EMaterialType GetMaterialType() const;
private:
	ITexture *Albedo;
};

class Metal : public IMaterial
{
public:
	Metal(const FVector& InAlbedo, float InFuzz) : Albedo(InAlbedo) { InFuzz < 1 ? Fuzz = InFuzz : Fuzz = 1; }

	virtual bool Scatter(const Ray& InRay, const FHit& Hit, FVector& Attenuation, Ray& Scattered) const;
	virtual EMaterialType GetMaterialType() const;
private:
	FVector Albedo;
	float Fuzz;
};

class Dielectric : public IMaterial
{
public:
	Dielectric(float InRefIndex) : RefIndex(InRefIndex) {}

	virtual bool Scatter(const Ray& InRay, const FHit& Hit, FVector& Attenuation, Ray& Scattered) const;
	virtual EMaterialType GetMaterialType() const;
private:
	float RefIndex;
};
