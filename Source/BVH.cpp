#include "BVH.h"
#include "utils/Random.h"
#include "utils/DebugPrint.h"
#include <algorithm>

BVHNode::BVHNode(IObject** List, int32 ListSize, float BeginTime, float EndTime)
{
	int32 Axis = int32(3 * Random::drand48());

	auto BoxCompare = [Axis](const IObject *AHit, const IObject *BHit) -> bool
	{
		AABB BoxL, BoxR;

		if (!AHit->BoundingBox(0, 0, BoxL) || !BHit->BoundingBox(0, 0, BoxR))
		{
			DebugPrint("No Bounding Box in BVHNode Constructor");
		}

		bool ReturnValue = false;

		switch (Axis)
		{
		case 0:
		{
			ReturnValue = (BoxL.Min().x - BoxR.Min().x) < 0.0f ? true : false;
			break;
		}
		case 1:
		{
			ReturnValue = (BoxL.Min().y - BoxR.Min().y) < 0.0f ? true : false;
			break;
		}
		default:
		{
			ReturnValue = (BoxL.Min().z - BoxR.Min().z) < 0.0f ? true : false;
			break;
		}
		}

		return ReturnValue;
	};

	std::sort(List, List + ListSize, BoxCompare);

	if (ListSize == 1)
	{
		Left = Right = List[0];
	}
	else if (ListSize == 2)
	{
		Left = List[0];
		Right = List[1];
	}
	else
	{
		Left = new BVHNode(List, ListSize / 2, BeginTime, EndTime);
		Right = new BVHNode(List + ListSize / 2, ListSize - ListSize / 2, BeginTime, EndTime);
	}

	AABB BoxLeft, BoxRight;

	if (!Left->BoundingBox(BeginTime, EndTime, BoxLeft) || !Right->BoundingBox(BeginTime, EndTime, BoxRight))
	{
		DebugPrint("No Bounding Box in BVHNode Constructor");
	}

	Box = SurroundingBox(BoxLeft, BoxRight);
}

bool BVHNode::Hit(const Ray& R, float TMin, float TMax, FHit& Hit) const
{
	if (Box.Hit(R, TMin, TMax))
	{
		FHit LeftHit, RightHit;
		bool bHitLeft = Left->Hit(R, TMin, TMax, LeftHit);
		bool bHitRight = Right->Hit(R, TMin, TMax, RightHit);

		if (bHitLeft && bHitRight)
		{
			if (LeftHit.T < RightHit.T)
			{
				Hit = LeftHit;
			}
			else
			{
				Hit = RightHit;
			}

			return true;
		}
		else if (bHitLeft)
		{
			Hit = LeftHit;
			return true;
		}
		else if (bHitRight)
		{
			Hit = RightHit;
			return true;
		}
		else
		{
			return false;
		}
	}

	return false;
}

bool BVHNode::BoundingBox(float T0, float T1, AABB& B) const
{
	B = Box;
	return true;
}

void BVHNode::Debug() const
{
	Left->Debug();
	Right->Debug();
	DebugPrint("BVHNode %x Left: %x, Right %x AABB: Min: %.2f,%.2f,%.2f Max: %.2f,%.2f,%.2f \n", this, Left, Right,
		Box.Min().x, Box.Min().y, Box.Min().z, Box.Max().x, Box.Max().y, Box.Max().z);
}
