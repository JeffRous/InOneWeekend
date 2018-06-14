#include "AABB.h"
#include <algorithm>

AABB SurroundingBox(AABB Box0, AABB Box1)
{
	FVector Small(fminf(Box0.Min.x, Box1.Min.x),
		fminf(Box0.Min.y, Box1.Min.y),
		fminf(Box0.Min.z, Box1.Min.y));

	FVector Big(fmaxf(Box0.Max.x, Box1.Max.x),
		fmaxf(Box0.Max.y, Box1.Max.y),
		fmaxf(Box0.Max.z, Box1.Max.y));

	return AABB(Small, Big);
}

bool AABB::Hit(const Ray& R, float TMin, float TMax) const
{
	for (int32 i = 0; i < 3; i++)
	{
		float InvD = 1.0f / R.GetDirection()[i];
		float T0 = (Min[i] - R.GetOrigin()[i]) * InvD;
		float T1 = (Max[i] - R.GetOrigin()[i]) * InvD;

		if (InvD < 0.0f)
		{
			std::swap(T1, T0);
		}

		TMin = T0 > TMin ? T0 : TMin;
		TMax = T1 < TMax ? T1 : TMax;

		if (TMax <= TMin)
		{
			return false;
		}
	}

	return true;
}
