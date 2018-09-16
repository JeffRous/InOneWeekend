#include "Object.h"
#include "utils/DebugPrint.h"

bool ObjectList::Hit(const Ray& R, float TMin, float TMax, FHit& Hit) const
{
	FHit Temp;
	bool bHit = false;

	float Closest = TMax;

	for (int32 i = 0; i < ListSize; i++)
	{
		if (List[i]->Hit(R, TMin, Closest, Temp))
		{
			bHit = true;
			Closest = Temp.T;
			Hit = Temp;
		}
	}

	return bHit;
}

bool ObjectList::BoundingBox(float T0, float T1, AABB& Box) const
{
	if (ListSize < 1)
	{
		return false;
	}

	AABB TempBox;

	bool FirstTrue = List[0]->BoundingBox(T0, T1, TempBox);

	if (!FirstTrue)
	{
		return false;
	}
	else
	{
		Box = TempBox;
	}

	for (int32 i = 1; i < ListSize; i++)
	{
		if (List[0]->BoundingBox(T0, T1, TempBox))
		{
			Box = SurroundingBox(Box, TempBox);
		}
		else
		{
			return false;
		}
	}
	return true;
}

void ObjectList::Debug() const
{
	DebugPrint(L"ObjectList %x ListSize: %d\n", this, ListSize);

	for (int32 i = 0; i < ListSize; i++)
	{
		List[i]->Debug();
	}
}
