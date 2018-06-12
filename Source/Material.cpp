#include "Material.h"
#include "utils/Random.h"

FVector RandomInUnitSphere()
{
	FVector P;

	do
	{
		P = 2.0f*FVector(float(Random::drand48()), float(Random::drand48()), float(Random::drand48())) - FVector(1, 1, 1);
	} while (P.SquaredLength() >= 1.0f);

	return P;
}

bool Lambertian::Scatter(const Ray& InRay, const FHit& Hit, FVector& Attenuation, Ray& Scattered) const
{
	FVector Target = Hit.P + Hit.Normal + RandomInUnitSphere();
	Scattered = Ray(Hit.P, Target - Hit.P);
	Attenuation = Albedo;
	return true;
}

static FVector Reflect(const FVector& V, const FVector& N)
{
	return V - 2 * Dot(V, N) * N;
}

bool Metal::Scatter(const Ray& InRay, const FHit& Hit, FVector& Attenuation, Ray& Scattered) const
{
	FVector Reflected = Reflect(UnitVector(InRay.GetDirection()), Hit.Normal);
	Scattered = Ray(Hit.P, Reflected + Fuzz * RandomInUnitSphere());
	Attenuation = Albedo;
	return Dot(Scattered.GetDirection(), Hit.Normal) > 0;
}
