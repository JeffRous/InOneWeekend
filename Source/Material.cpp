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

static FVector Reflect(const FVector& V, const FVector& N)
{
	return V - 2 * Dot(V, N) * N;
}

static float Schlick(float Cosine, float RefIndex)
{
	float R0 = (1 - RefIndex) / (1 + RefIndex);
	R0 = R0 * R0;
	return R0 + (1 - R0) * powf((1 - Cosine), 5);
}

static bool Refract(const FVector& V, const FVector& N, float NiOverNt, FVector& Refracted)
{
	FVector UV = UnitVector(V);
	float DT = Dot(UV, N);
	float Discriminant = 1.0f - NiOverNt * NiOverNt*(1.0f - DT * DT);

	if (Discriminant > 0)
	{
		Refracted = NiOverNt * (UV - N * DT) - N * sqrtf(Discriminant);
		return true;
	}
	else
	{
		return false;
	}
}

bool MaterialScatter(const Material* Mat, const Ray& InRay, const FHit& Hit, FVector& Attenuation, Ray& Scattered)
{
	if (Mat->Type == EMaterialType::Lambertian)
	{
		FVector Target = Hit.P + Hit.Normal + RandomInUnitSphere();
		Scattered = Ray(Hit.P, Target - Hit.P, InRay.GetTime());
		Attenuation = Sample(Mat->Albedo, 0, 0, Hit.P);
		return true;
	}
	else if (Mat->Type == EMaterialType::Metal)
	{
		FVector Reflected = Reflect(UnitVector(InRay.GetDirection()), Hit.Normal);
		Scattered = Ray(Hit.P, Reflected + Mat->Roughness * RandomInUnitSphere(), InRay.GetTime());
		Attenuation = Sample(Mat->Albedo, 0, 0, Hit.P);
		return Dot(Scattered.GetDirection(), Hit.Normal) > 0;
	}
	else if (Mat->Type == EMaterialType::Dielectric)
	{
		FVector OutwardNormal;
		FVector Reflected = Reflect(InRay.GetDirection(), Hit.Normal);
		float NiOverNt;
		Attenuation = FVector(1.0f, 1.0f, 1.0f);
		FVector Refracted;
		float ReflectProbability;
		float Cosine;

		float RayDirectionDotNormal = Dot(InRay.GetDirection(), Hit.Normal);

		if (RayDirectionDotNormal > 0)
		{
			OutwardNormal = -Hit.Normal;
			NiOverNt = Mat->Ri;
			Cosine = Mat->Ri * RayDirectionDotNormal / InRay.GetDirection().Length();
		}
		else
		{
			OutwardNormal = Hit.Normal;
			NiOverNt = 1.0f / Mat->Ri;
			Cosine = -RayDirectionDotNormal / InRay.GetDirection().Length();
		}

		if (Refract(InRay.GetDirection(), OutwardNormal, NiOverNt, Refracted))
		{
			ReflectProbability = Schlick(Cosine, Mat->Ri);
		}
		else
		{
			ReflectProbability = 1.0f;
		}

		if (Random::drand48() < ReflectProbability)
		{
			Scattered = Ray(Hit.P, Reflected, InRay.GetTime());
		}
		else
		{
			Scattered = Ray(Hit.P, Refracted, InRay.GetTime());
		}

		return true;
	}

	return false;
}

bool Lambertian::Scatter(const Ray& InRay, const FHit& Hit, FVector& Attenuation, Ray& Scattered) const
{
	return MaterialScatter(&Mat, InRay, Hit, Attenuation, Scattered);
}

bool Metal::Scatter(const Ray& InRay, const FHit& Hit, FVector& Attenuation, Ray& Scattered) const
{
	return MaterialScatter(&Mat, InRay, Hit, Attenuation, Scattered);
}

bool Dielectric::Scatter(const Ray& InRay, const FHit& Hit, FVector& Attenuation, Ray& Scattered) const
{
	return MaterialScatter(&Mat, InRay, Hit, Attenuation, Scattered);
}
