#pragma once

#include "RayTracing_ispc.h"
#include <algorithm>

#define ALIGNMENT 32

/* Keep these around if not using ISPC
enum class EObjectType
{
	BVH = 0,
	Sphere,
	MovingSphere
};

enum class EMaterialType
{
	None = 0,
	Lambertian,
	Dielectric,
	Metal
};

enum class ETextureType
{
	No = 0,
	Constant,
	Checker
};

struct Texture
{
	ETextureType Type;
	FVector Color;

	FVector ColorEven;
	FVector ColorOdd;
};

struct Material
{
	EMaterialType Type;
	Texture Albedo;
	float Ri;
	float Roughness;
};

struct Object
{
	EObjectType Type;

	Material Mat;
	FVector Center0, Center1;
	float Time0, Time1;
	float Radius;
};

struct BoundingBox
{
	FVector Min;
	FVector Max;
};

struct ISPCBVHNode
{
	BoundingBox Box;

	EObjectType ObjectType;
	Object *Obj;

	ISPCBVHNode* Parent;
	ISPCBVHNode* Left;
	ISPCBVHNode* Right;

	//void Debug() const;
};
*/
