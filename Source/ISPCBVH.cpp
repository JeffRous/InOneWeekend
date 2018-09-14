#include "ISPCBVH.h"
#include "utils/Random.h"
#include "utils/DebugPrint.h"
#include <algorithm>
#include <stack>
#include <assert.h>

ISPCBVHNode *CreateISPCBVHNode(IObject** List, int32 ListSize, ISPCBVHNode* Parent, float BeginTime, float EndTime)
{
	ISPCBVHNode *Node = new ISPCBVHNode;
	Node->ObjectType = EObjectType::BVH;
	Node->MaterialType = EMaterialType::None;
	Node->Left = nullptr;
	Node->Right = nullptr;
	Node->Object = nullptr;
	Node->Parent = Parent;

	if (ListSize == 1)
	{
		List[0]->BoundingBox(BeginTime, EndTime, Node->Box);
		Node->ObjectType = List[0]->GetObjectType();
		Node->MaterialType = List[0]->GetMaterialType();
		Node->Object = List[0];
	}
	else if (ListSize == 2)
	{
	Node->Left = CreateISPCBVHNode(&List[0], 1, Node, BeginTime, EndTime);
	Node->Right = CreateISPCBVHNode(&List[1], 1, Node, BeginTime, EndTime);
	}
	else
	{
	Node->Left = CreateISPCBVHNode(List, ListSize / 2, Node, BeginTime, EndTime);
	Node->Right = CreateISPCBVHNode(List + ListSize / 2, ListSize - ListSize / 2, Node, BeginTime, EndTime);
	}

	if (Node->Left != nullptr && Node->Right != nullptr)
	{
		Node->Box = SurroundingBox(Node->Left->Box, Node->Right->Box);
	}

	return Node;
}

ISPCBVH::ISPCBVH(IObject** List, int32 ListSize, float BeginTime, float EndTime)
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

	RootNode = CreateISPCBVHNode(List, ListSize, nullptr, BeginTime, EndTime);
}

FVector GetCenterAt(ISPCBVHNode *Node, float Time)
{
	if (Node->ObjectType == EObjectType::Sphere)
	{
		return Node->Obj.Center0;
	}
	else if (Node->ObjectType == EObjectType::MovingSphere)
	{
		return Node->Obj.Center0 + ((Time - Node->Obj.Time0) / (Node->Obj.Time1 - Node->Obj.Time0)) * (Node->Obj.Center1 - Node->Obj.Center0);
	}

	return FVector(0, 0, 0);
}

bool RayIntersect(const Ray& R, ISPCBVHNode *Node, float TMin, float TMax, FHit& Hit)
{
	/*
	FVector GetCenterAtTime = GetCenterAt(Node, R.GetTime());
	FVector RayDirection = R.GetDirection();
	FVector Oc = R.GetOrigin() - GetCenterAtTime;

	float a = Dot(RayDirection, RayDirection);
	float b = Dot(Oc, RayDirection);
	float c = Dot(Oc, Oc) - Node->Obj.Radius * Node->Obj.Radius;
	float Discriminant = b * b - a * c;

	if (Discriminant > 0)
	{
		float DiscSqrt = sqrtf(Discriminant);
		float Temp = (-b - DiscSqrt) / a;

		auto SetHit = [&]()
		{
			Hit.T = Temp;
			Hit.P = R.PointAtT(Temp);
			Hit.Normal = (Hit.P - GetCenterAtTime) / Node->Obj.Radius;
			Hit.MaterialType = Node->MaterialType;
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
	*/
	return Node->Object->Hit(R, TMin, TMax, Hit);
}

ISPCBVHNode* Sibling(ISPCBVHNode *Current)
{
	if (Current->Parent == nullptr)
	{
		return nullptr;
	}

	return Current->Parent->Right;
}

ISPCBVHNode* Parent(ISPCBVHNode *Current)
{
	return Current->Parent;
}

ISPCBVHNode* NearChild(ISPCBVHNode *Current)
{
	return Current->Left;
}

bool ISPCBVH::Hit(const Ray& R, float TMin, float TMax, FHit& Hit) const
{
	enum class ETraveralState
	{
		FromParent = 0,
		FromChild,
		FromSibling
	};

	Hit.T = FLT_MAX;

	ISPCBVHNode *Current = RootNode;
	ETraveralState State = ETraveralState::FromParent;

	bool bHit = false;

	while (true)
	{
		switch (State)
		{
		case ETraveralState::FromChild:
		{
			if (Current == RootNode)
			{
				if (bHit)
				{
					return true;
				}
				else
				{
					return false;
				}
			}
			if (Current == NearChild(Parent(Current)))
			{
				Current = Sibling(Current);
				State = ETraveralState::FromSibling;
			}
			else
			{
				Current = Parent(Current);
				State = ETraveralState::FromChild;
			}
			break;
		}
		case ETraveralState::FromSibling:
		{
			if (!Current->Box.Hit(R, TMin, TMax))
			{
				Current = Parent(Current);
				State = ETraveralState::FromChild;
			}
			else if (Current->ObjectType != EObjectType::BVH)
			{
				FHit PreviousHit = Hit;
				bool bPreviouslyHit = bHit;

				bHit = RayIntersect(R, Current, TMin, TMax, Hit);

				if (bHit && PreviousHit.T < Hit.T)
				{
					Hit = PreviousHit;
				}

				if (bPreviouslyHit && !bHit)
				{
					bHit = true;
				}

				Current = Parent(Current);
				State = ETraveralState::FromChild;
			}
			else
			{
				Current = NearChild(Current);
				State = ETraveralState::FromParent;
			}
			break;
		}
		case ETraveralState::FromParent:
		{
			if (!Current->Box.Hit(R, TMin, TMax))
			{
				if (Sibling(Current) == nullptr)
				{
					if (Parent(Current) == nullptr)
					{
						State = ETraveralState::FromChild;
					}
					else
					{
						Current = Parent(Current);
						State = ETraveralState::FromChild;
					}
				}
				else
				{
					Current = Sibling(Current);
					State = ETraveralState::FromSibling;
				}
			}
			else if (Current->ObjectType != EObjectType::BVH)
			{
				FHit PreviousHit = Hit;
				bool bPreviouslyHit = bHit;

				bHit = RayIntersect(R, Current, TMin, TMax, Hit);

				if (bHit && PreviousHit.T < Hit.T)
				{
					Hit = PreviousHit;
				}

				if (bPreviouslyHit && !bHit)
				{
					bHit = true;
				}

				if (Sibling(Current) == nullptr)
				{
					if (Parent(Current) == nullptr)
					{
						State = ETraveralState::FromChild;
					}
					else
					{
						Current = Parent(Current);
						State = ETraveralState::FromChild;
					}
				}
				else
				{
					Current = Sibling(Current);
					State = ETraveralState::FromSibling;
				}
			}
			else
			{
				Current = NearChild(Current);
				State = ETraveralState::FromParent;
			}
			break;
		}
		}
	}
}

bool ISPCBVH::BoundingBox(float T0, float T1, AABB& B) const
{
	B = RootNode->Box;
	return true;
}

EObjectType ISPCBVH::GetObjectType() const
{
	return EObjectType::BVH;
}

EMaterialType ISPCBVH::GetMaterialType() const
{
	return EMaterialType::None;
}

void ISPCBVH::Debug() const
{
	RootNode->Debug();
}

void ISPCBVHNode::Debug() const
{
	if (Left)
	{
		Left->Debug();
	}
	if (Right)
	{
		Right->Debug();
	}

	DebugPrint("%s %x Left: %x, Right %x AABB: Min: %.2f,%.2f,%.2f Max: %.2f,%.2f,%.2f \n", ObjectType == EObjectType::BVH ? "ISPCBVHNode" : "Object", this, Left, Right,
		Box.Min().x, Box.Min().y, Box.Min().z, Box.Max().x, Box.Max().y, Box.Max().z);
}
