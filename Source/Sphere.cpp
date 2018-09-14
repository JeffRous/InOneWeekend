#include "Sphere.h"
#include "utils/DebugPrint.h"

bool Sphere::Hit(const Ray& R, float TMin, float TMax, FHit& Hit) const
{
	FVector Oc = R.GetOrigin() - Center;
	float a = Dot(R.GetDirection(), R.GetDirection());
	float b = Dot(Oc, R.GetDirection());
	float c = Dot(Oc, Oc) - Radius*Radius;
	float Discriminant = b*b - a*c;

	if (Discriminant > 0)
	{
		float DiscSqrt = sqrtf(Discriminant);
		float Temp = (-b - DiscSqrt) / a;

		auto SetHit = [&]()
		{
			Hit.T = Temp;
			Hit.P = R.PointAtT(Temp);
			Hit.Normal = (Hit.P - Center) / Radius;
			Hit.MaterialType = GetMaterialType();
			Hit.Material = IMat;
		};

		if (Temp < TMax && Temp > TMin)
		{
			SetHit();
			return true;
		}

		Temp = (-b + DiscSqrt) / a;

		if (Temp < TMax && Temp > TMin)
		{
			SetHit();
			return true;
		}
	}

	return false;
}

bool Sphere::BoundingBox(float T0, float T1, AABB& Box) const
{
	Box = AABB(Center - FVector(Radius, Radius, Radius), Center + FVector(Radius, Radius, Radius));
	return true;
}

EObjectType Sphere::GetObjectType() const
{
	return EObjectType::Sphere;
}

EMaterialType Sphere::GetMaterialType() const
{
	return IMat->GetMaterialType();
}

void Sphere::Debug() const
{
	DebugPrint("Sphere %x Center: %.2f,%.2f,%.2f Radius: %.2f\n", this, Center.x, Center.y, Center.z, Radius);
}

bool MovingSphere::Hit(const Ray& R, float TMin, float TMax, FHit& Hit) const
{
	FVector GetCenterAtTime = GetCenterAt(R.GetTime());
	FVector RayDirection = R.GetDirection();
	FVector Oc = R.GetOrigin() - GetCenterAtTime;

	float a = Dot(RayDirection, RayDirection);
	float b = Dot(Oc, RayDirection);
	float c = Dot(Oc, Oc) - Radius * Radius;
	float Discriminant = b * b - a * c;

	if (Discriminant > 0)
	{
		float DiscSqrt = sqrtf(Discriminant);
		float Temp = (-b - DiscSqrt) / a;

		auto SetHit = [&]()
		{
			Hit.T = Temp;
			Hit.P = R.PointAtT(Temp);
			Hit.Normal = (Hit.P - GetCenterAtTime) / Radius;
			Hit.MaterialType = GetMaterialType();
			Hit.Material = IMat;
		};

		if (Temp < TMax && Temp > TMin)
		{
			SetHit();
			return true;
		}

		Temp = (-b + DiscSqrt) / a;

		if (Temp < TMax && Temp > TMin)
		{
			SetHit();
			return true;
		}
	}

	return false;
}

bool MovingSphere::BoundingBox(float T0, float T1, AABB& Box) const
{
	FVector CenterT0 = GetCenterAt(T0);
	FVector CenterT1 = GetCenterAt(T1);

	AABB Box0 = AABB(CenterT0 - FVector(Radius, Radius, Radius), CenterT0 + FVector(Radius, Radius, Radius));
	AABB Box1 = AABB(CenterT1 - FVector(Radius, Radius, Radius), CenterT1 + FVector(Radius, Radius, Radius));
	Box = SurroundingBox(Box0, Box1);
	return true;
}

EObjectType MovingSphere::GetObjectType() const
{
	return EObjectType::MovingSphere;
}

EMaterialType MovingSphere::GetMaterialType() const
{
	return IMat->GetMaterialType();
}

FVector MovingSphere::GetCenterAt(float Time) const
{
	return Center0 + ((Time - Time0) / (Time1 - Time0)) * (Center1 - Center0);
}

void MovingSphere::Debug() const
{
	DebugPrint("MovingSphere %x Center0: %.2f,%.2f,%.2f Center1: %.2f,%.2f,%.2f Time0: %.2f Time1: %.2f Radius: %.2f\n",
		this, Center0.x, Center0.y, Center0.z, Center1.x, Center1.y, Center1.z, Time0, Time1, Radius);
}
