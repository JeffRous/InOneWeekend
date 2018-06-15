#include "AABB.h"
#include <algorithm>

AABB SurroundingBox(AABB Box0, AABB Box1)
{
	FVector Small(fmin(Box0.Min().x, Box1.Min().x),
		fmin(Box0.Min().y, Box1.Min().y),
		fmin(Box0.Min().z, Box1.Min().z));

	FVector Big(fmax(Box0.Max().x, Box1.Max().x),
		fmax(Box0.Max().y, Box1.Max().y),
		fmax(Box0.Max().z, Box1.Max().z));

	return AABB(Small, Big);
}

bool AABB::Hit(const Ray& R, float TMin, float TMax) const
{
	for (int32 i = 0; i < 3; i++)
	{
		float InvD = 1.0f / R.GetDirection()[i];
		float T0 = (Min()[i] - R.GetOrigin()[i]) * InvD;
		float T1 = (Max()[i] - R.GetOrigin()[i]) * InvD;

		if (InvD < 0.0f)
		{
			std::swap(T0, T1);
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
