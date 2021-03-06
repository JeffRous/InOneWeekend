#include "Sphere.h"
#include "utils/DebugPrint.h"

bool Sphere::Hit(const Ray& R, float TMin, float TMax, FHit& Hit) const
{
	FVector Oc = R.GetOrigin() - Obj.Center0;
	float a = Dot(R.GetDirection(), R.GetDirection());
	float b = Dot(Oc, R.GetDirection());
	float c = Dot(Oc, Oc) - Obj.Radius*Obj.Radius;
	float Discriminant = b*b - a*c;

	if (Discriminant > 0)
	{
		float DiscSqrt = sqrtf(Discriminant);
		float Temp = (-b - DiscSqrt) / a;

		auto SetHit = [&]()
		{
			Hit.T = Temp;
			Hit.P = R.PointAtT(Temp);
			Hit.Normal = (Hit.P - Obj.Center0) / Obj.Radius;
			Hit.Mat = &Obj.Mat;
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
	Box = AABB(Obj.Center0 - FVector(Obj.Radius, Obj.Radius, Obj.Radius), Obj.Center0 + FVector(Obj.Radius, Obj.Radius, Obj.Radius));
	return true;
}

void Sphere::Debug() const
{
	DebugPrint(L"Sphere %x Center: %.2f,%.2f,%.2f Radius: %.2f\n", this, Obj.Center0.v[0], Obj.Center0.v[1], Obj.Center0.v[2], Obj.Radius);
}

bool MovingSphere::Hit(const Ray& R, float TMin, float TMax, FHit& Hit) const
{
	FVector GetCenterAtTime = GetCenterAt(R.GetTime());
	FVector RayDirection = R.GetDirection();
	FVector Oc = R.GetOrigin() - GetCenterAtTime;

	float a = Dot(RayDirection, RayDirection);
	float b = Dot(Oc, RayDirection);
	float c = Dot(Oc, Oc) - Obj.Radius * Obj.Radius;
	float Discriminant = b * b - a * c;

	if (Discriminant > 0)
	{
		float DiscSqrt = sqrtf(Discriminant);
		float Temp = (-b - DiscSqrt) / a;

		auto SetHit = [&]()
		{
			Hit.T = Temp;
			Hit.P = R.PointAtT(Temp);
			Hit.Normal = (Hit.P - GetCenterAtTime) / Obj.Radius;
			Hit.Mat = &Obj.Mat;
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

	AABB Box0 = AABB(CenterT0 - FVector(Obj.Radius, Obj.Radius, Obj.Radius), CenterT0 + FVector(Obj.Radius, Obj.Radius, Obj.Radius));
	AABB Box1 = AABB(CenterT1 - FVector(Obj.Radius, Obj.Radius, Obj.Radius), CenterT1 + FVector(Obj.Radius, Obj.Radius, Obj.Radius));
	Box = SurroundingBox(Box0, Box1);
	return true;
}

FVector MovingSphere::GetCenterAt(float Time) const
{
	return Obj.Center0 + ((Time - Obj.Time0) / (Obj.Time1 - Obj.Time0)) * (Obj.Center1 - Obj.Center0);
}

void MovingSphere::Debug() const
{
	DebugPrint(L"MovingSphere %x Center0: %.2f,%.2f,%.2f Center1: %.2f,%.2f,%.2f Time0: %.2f Time1: %.2f Radius: %.2f\n",
		this, Obj.Center0.v[0], Obj.Center0.v[1], Obj.Center0.v[2], Obj.Center1.v[0], Obj.Center1.v[1], Obj.Center1.v[2], Obj.Time0, Obj.Time1, Obj.Radius);
}
