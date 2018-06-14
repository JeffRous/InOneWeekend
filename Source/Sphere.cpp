#include "Sphere.h"

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
			Hit.Material = Material;
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

bool MovingSphere::Hit(const Ray& R, float TMin, float TMax, FHit& Hit) const
{
	FVector Oc = R.GetOrigin() - Center(R.GetTime());
	float a = Dot(R.GetDirection(), R.GetDirection());
	float b = Dot(Oc, R.GetDirection());
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
			Hit.Normal = (Hit.P - Center(R.GetTime())) / Radius;
			Hit.Material = Material;
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
	FVector CenterT0 = Center(T0);
	FVector CenterT1 = Center(T1);

	AABB Box0 = AABB(CenterT0 - FVector(Radius, Radius, Radius), CenterT0 + FVector(Radius, Radius, Radius));
	AABB Box1 = AABB(CenterT1 - FVector(Radius, Radius, Radius), CenterT1 + FVector(Radius, Radius, Radius));
	Box = SurroundingBox(Box0, Box1);
	return true;
}

FVector MovingSphere::Center(float Time) const
{
	return CenterBegin + ((Time - BeginTime) / (EndTime - BeginTime)) * (CenterEnd - CenterBegin);
}
