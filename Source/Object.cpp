#include "Object.h"

bool ObjectList::Hit(const Ray& R, float TMin, float TMax, FHit& Hit) const
{
	FHit Temp;
	bool bHit = false;

	float Closest = TMax;

	for (int32 i = 0; i < ListNumber; i++)
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
