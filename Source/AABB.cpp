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

BoundingBox SurroundingBox(BoundingBox Box0, BoundingBox Box1)
{
	AABB Small = AABB(Box0.Min, Box0.Max);
	AABB Big = AABB(Box1.Min, Box1.Max);

	AABB EnclosingBox = SurroundingBox(Small, Big);

	BoundingBox ReturnValue;
	ReturnValue.Min = EnclosingBox.Min();
	ReturnValue.Max = EnclosingBox.Max();

	return ReturnValue;
}

bool IntersectBox(const BoundingBox& B, const Ray& R, float TMin, float TMax)
{
	FVector Direction = R.GetDirection();
	FVector Origin = R.GetOrigin();

	for (int32 i = 0; i < 3; i++)
	{
		float InvD = 1.0f / Direction[i];
		float T0 = (B.Min[i] - Origin[i]) * InvD;
		float T1 = (B.Max[i] - Origin[i]) * InvD;

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

bool AABB::Hit(const Ray& R, float TMin, float TMax) const
{
	return IntersectBox(Box, R, TMin, TMax);
}
