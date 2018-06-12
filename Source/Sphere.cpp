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
