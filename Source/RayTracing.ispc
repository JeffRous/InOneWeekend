//typedef float<3> float3;

#define FLT_MAX          3.402823466e+38F        // max value
#define MAX_BOUNCES 50

static RNGState rngstate;

struct float3
{
	float e[3];
};

enum EObjectType
{
	BVH = 0,
	Sphere,
	MovingSphere
};

enum EMaterialType
{
	None = 0,
	Lambertian,
	Dielectric,
	Metal
};

enum ETextureType
{
	No = 0,
	Constant,
	Checker
};

struct Texture
{
	ETextureType Type;
	float3 Color;

	float3 ColorEven;
	float3 ColorOdd;
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
	float3 Center0, Center1;
	float Time0, Time1;
	float Radius;
};

struct BoundingBox
{
	float3 Min;
	float3 Max;
};

struct ISPCBVHNode
{
	BoundingBox Box;

	EObjectType ObjectType;
	Object *Obj;

	ISPCBVHNode* Parent;
	ISPCBVHNode* Left;
	ISPCBVHNode* Right;
};

struct FHit
{
	float T; // Ray parameter
	float3 P; // 3D Point on ray
	float3 Normal;
	const Material *Mat;
};

struct FCamera
{
	float3 Origin;
	float3 LowerLeft;
	float3 Horizontal;
	float3 Vertical;
	float3 U, V, W;
	float LensRadius;
	float BeginTime, EndTime;
};

struct Ray
{
	float3 Origin;
	float3 Direction;
	float Time;
};

static void swap(float& a, float& b)
{
    int t = a;
    a = b; b = t;
}

static inline float dot(float3 v1, float3 v2)
{
	return v1.e[0] * v2.e[0] + v1.e[1] * v2.e[1] + v1.e[2] * v2.e[2];
}

static inline float3 mul(float3 v, float t)
{
	float3 ret;
	ret.e[0] = v.e[0] * t;
	ret.e[1] = v.e[1] * t;
	ret.e[2] = v.e[2] * t;
	return ret;
}

static inline float3 mul(float3 v1, float3 v2)
{
	float3 ret;
	ret.e[0] = v1.e[0] * v2.e[0];
	ret.e[1] = v1.e[1] * v2.e[1];
	ret.e[2] = v1.e[2] * v2.e[2];
	return ret;
}

static inline float3 add(float3 v, float t)
{
	float3 ret;
	ret.e[0] = v.e[0] + t;
	ret.e[1] = v.e[1] + t;
	ret.e[2] = v.e[2] + t;
	return ret;
}

static inline float3 add(float3 v1, float3 v2)
{
	float3 ret;
	ret.e[0] = v1.e[0] + v2.e[0];
	ret.e[1] = v1.e[1] + v2.e[1];
	ret.e[2] = v1.e[2] + v2.e[2];
	return ret;
}

static inline float3 sub(float3 v1, float3 v2)
{
	float3 ret;
	ret.e[0] = v1.e[0] - v2.e[0];
	ret.e[1] = v1.e[1] - v2.e[1];
	ret.e[2] = v1.e[2] - v2.e[2];
	return ret;
}

inline float3 operator+(const float3 v1, const float3 v2)
{
	float3 ret;
	ret.e[0] = v1.e[0] + v2.e[0];
	ret.e[1] = v1.e[1] + v2.e[1];
	ret.e[2] = v1.e[2] + v2.e[2];
	return ret;
}

inline float3 operator-(const float3 v1, const float3 v2)
{
	float3 ret;
	ret.e[0] = v1.e[0] - v2.e[0];
	ret.e[1] = v1.e[1] - v2.e[1];
	ret.e[2] = v1.e[2] - v2.e[2];
	return ret;
}

inline float3 operator*(const float3 v1, const float3 v2)
{
	float3 ret;
	ret.e[0] = v1.e[0] * v2.e[0];
	ret.e[1] = v1.e[1] * v2.e[1];
	ret.e[2] = v1.e[2] * v2.e[2];
	return ret;
}

inline float3 operator/(const float3 v1, const float3 v2)
{
	float3 ret;
	ret.e[0] = v1.e[0] / v2.e[0];
	ret.e[1] = v1.e[1] / v2.e[1];
	ret.e[2] = v1.e[2] / v2.e[2];
	return ret;
}

inline float3 operator*(const float3 v, float t)
{
	float3 ret;
	ret.e[0] = v.e[0] * t;
	ret.e[1] = v.e[1] * t;
	ret.e[2] = v.e[2] * t;
	return ret;
}

inline float3 operator*(float t, const float3 v)
{
	float3 ret;
	ret.e[0] = v.e[0] * t;
	ret.e[1] = v.e[1] * t;
	ret.e[2] = v.e[2] * t;
	return ret;
}

inline float3 operator/(const float3 v, float t)
{
	float3 ret;
	ret.e[0] = v.e[0] / t;
	ret.e[1] = v.e[1] / t;
	ret.e[2] = v.e[2] / t;
	return ret;
}

inline float3 operator/(float t, const float3 v)
{
	float3 ret;
	ret.e[0] = t / v.e[0];
	ret.e[1] = t / v.e[1];
	ret.e[2] = t / v.e[2];
	return ret;
}

inline float3 assign(float x, float y, float z)
{
	float3 ret;
	ret.e[0] = x;
	ret.e[1] = y;
	ret.e[2] = z;
	return ret;
}

inline float Length(float3 v)
{
	return sqrt(v.e[0] * v.e[0] + v.e[1] * v.e[1] + v.e[2] * v.e[2]);
}

inline float SquaredLength(float3 v)
{
	return v.e[0] * v.e[0] + v.e[1] * v.e[1] + v.e[2] * v.e[2];
}

inline float3 UnitVector(float3 v)
{
	return v / Length(v);
}

inline Ray MakeRay(float3 Origin, float3 Direction, float Time)
{
	Ray R;
	R.Origin = Origin;
	R.Direction = Direction;
	R.Time = Time;
	return R;
}

static inline float drand48()
{
	return frandom(&rngstate);
}

export void Random(uniform float Randoms[], uniform unsigned int NumRandoms)
{
	seed_rng(&rngstate, programIndex + (NumRandoms << (programIndex & 15)));

	foreach(i = 0 ... NumRandoms)
	{
		Randoms[i] = frandom(&rngstate);
	}
}

static inline float3 RandomInUnitDisk()
{
	float3 P;

	do
	{
		P.e[0] = (2.0f * drand48()) - 1.0f;
		P.e[1] = (2.0f * drand48()) - 1.0f;
		P.e[2] = 0.0f;
	} while (dot(P,P) >= 1.0f);

	return P;
}

static inline float3 RandomInUnitSphere()
{
	float3 P;

	do
	{
		P = 2.0f*assign(drand48(), drand48(), drand48()) - assign(1, 1, 1);
	} while (SquaredLength(P) >= 1.0f);

	return P;
}

export void GetRays(uniform Ray Rays[], uniform FCamera &Camera, uniform int Samples, uniform int i, uniform int Width, uniform int j, uniform int Height)
{
	seed_rng(&rngstate, programIndex + (i << (programIndex & 15)));

	foreach(s = 0 ... Samples)
	{
		float u = (i + frandom(&rngstate)) / Width;
		float v = (j + frandom(&rngstate)) / Height;

		float3 RandomPointOnLens = mul(RandomInUnitDisk(), Camera.LensRadius);
		float3 Offset = add(mul(Camera.U, RandomPointOnLens.e[0]), mul(Camera.V, RandomPointOnLens.e[1]));

		Rays[s].Origin = add(Camera.Origin, Offset);
		Rays[s].Direction = sub(sub(add(add(mul(Camera.Horizontal, u), Camera.LowerLeft), mul(Camera.Vertical, v)), Camera.Origin), Offset);
		Rays[s].Time = Camera.BeginTime + frandom(&rngstate)*(Camera.EndTime - Camera.BeginTime);
	}
}

bool IntersectBox(const BoundingBox* B, const Ray& R, float TMin, float TMax)
{
	float3 Direction = R.Direction;
	float3 Origin = R.Origin;

	for (int32 i = 0; i < 3; i++)
	{
		float InvD = 1.0f / Direction.e[i];
		float T0 = (B->Min.e[i] - Origin.e[i]) * InvD;
		float T1 = (B->Max.e[i] - Origin.e[i]) * InvD;

		if (InvD < 0.0f)
		{
			swap(T0, T1);
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

float3 Sample(const Texture* T, const float3& p)
{
	if (T->Type == Constant)
	{
		return T->Color;
	}
	else if (T->Type == Checker)
	{
		float Sines = sin(10 * p.e[0])*sin(10 * p.e[1])*sin(10 * p.e[2]);

		if (Sines < 0)
		{
			return T->ColorOdd;
		}
		else
		{
			return T->ColorEven;
		}
	}
	else
	{
		return assign(0,0,0);
	}
}

float3 GetCenterAt(ISPCBVHNode *Node, float Time)
{
	if (Node->Obj->Type == Sphere)
	{
		return Node->Obj->Center0;
	}
	else if (Node->Obj->Type == MovingSphere)
	{
		return Node->Obj->Center0 + ((Time - Node->Obj->Time0) / (Node->Obj->Time1 - Node->Obj->Time0)) * (Node->Obj->Center1 - Node->Obj->Center0);
	}

	return assign(0,0,0);
}

bool RayIntersect(const Ray& R, ISPCBVHNode *Node, float TMin, float TMax, FHit& Hit)
{
	float3 GetCenterAtTime = GetCenterAt(Node, R.Time);
	float3 Oc = R.Origin - GetCenterAtTime;

	float a = dot(R.Direction, R.Direction);
	float b = dot(Oc, R.Direction);
	float c = dot(Oc, Oc) - Node->Obj->Radius * Node->Obj->Radius;
	float Discriminant = b * b - a * c;

	if (Discriminant > 0)
	{
		float DiscSqrt = sqrt(Discriminant);
		float Temp = (-b - DiscSqrt) / a;

		if (Temp < TMax && Temp > TMin)
		{
			Hit.T = Temp;
			Hit.P = R.Origin + Temp * R.Direction;
			Hit.Normal = (Hit.P - GetCenterAtTime) / Node->Obj->Radius;
			Hit.Mat = &Node->Obj->Mat;

			return true;
		}

		Temp = (-b + DiscSqrt) / a;

		if (Temp < TMax && Temp > TMin)
		{
			Hit.T = Temp;
			Hit.P = R.Origin + Temp * R.Direction;
			Hit.Normal = (Hit.P - GetCenterAtTime) / Node->Obj->Radius;
			Hit.Mat = &Node->Obj->Mat;

			return true;
		}
	}

	return false;
}

ISPCBVHNode* Sibling(ISPCBVHNode *Current)
{
	ISPCBVHNode *Parent = Current->Parent;

	if (Parent == 0)
	{
		return 0;
	}

	return Parent->Right;
}

ISPCBVHNode* Parent(ISPCBVHNode *Current)
{
	return Current->Parent;
}

ISPCBVHNode* NearChild(ISPCBVHNode *Current)
{
	return Current->Left;
}

bool Traverse(ISPCBVHNode* RootNode, const Ray& R, float TMin, float TMax, FHit& Hit)
{
	enum ETraveralState
	{
		FromParent = 0,
		FromChild,
		FromSibling
	};

	Hit.T = FLT_MAX;

	ISPCBVHNode *Current = RootNode;
	ETraveralState State = FromParent;

	bool bHit = false;

	while (true)
	{
		switch (State)
		{
		case FromChild:
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
				State = FromSibling;
			}
			else
			{
				Current = Parent(Current);
				State = FromChild;
			}
			break;
		}
		case FromSibling:
		{
			if (!IntersectBox(&Current->Box, R, TMin, TMax))
			{
				Current = Parent(Current);
				State = FromChild;
			}
			else if (Current->ObjectType != BVH)
			{
				FHit PreviousHit = Hit;
				bool bPreviouslyHit = bHit;

				bHit = RayIntersect(R, Current, TMin, TMax, Hit);

				if(bHit)
				{
					print("Hit in FromSibling");
				}

				if (bHit && PreviousHit.T < Hit.T)
				{
					Hit = PreviousHit;
				}

				if (bPreviouslyHit && !bHit)
				{
					bHit = true;
				}

				Current = Parent(Current);
				State = FromChild;
			}
			else
			{
				Current = NearChild(Current);
				State = FromParent;
			}
			break;
		}
		case FromParent:
		{
			if (!IntersectBox(&Current->Box, R, TMin, TMax))
			{
				if (Sibling(Current) == 0)
				{
					if (Parent(Current) == 0)
					{
						State = FromChild;
					}
					else
					{
						Current = Parent(Current);
						State = FromChild;
					}
				}
				else
				{
					Current = Sibling(Current);
					State = FromSibling;
				}
			}
			else if (Current->ObjectType != BVH)
			{
				FHit PreviousHit = Hit;
				bool bPreviouslyHit = bHit;

				bHit = RayIntersect(R, Current, TMin, TMax, Hit);

				if(bHit)
				{
					print("Hit in FromParent");
				}

				if (bHit && PreviousHit.T < Hit.T)
				{
					Hit = PreviousHit;
				}

				if (bPreviouslyHit && !bHit)
				{
					bHit = true;
				}

				if (Sibling(Current) == 0)
				{
					if (Parent(Current) == 0)
					{
						State = FromChild;
					}
					else
					{
						Current = Parent(Current);
						State = FromChild;
					}
				}
				else
				{
					Current = Sibling(Current);
					State = FromSibling;
				}
			}
			else
			{
				Current = NearChild(Current);
				State = FromParent;
			}
			break;
		}
		}
	}
}

static float3 Reflect(const float3& V, const float3& N)
{
	return V - 2 * dot(V, N) * N;
}

static float Schlick(float Cosine, float RefIndex)
{
	float R0 = (1 - RefIndex) / (1 + RefIndex);
	R0 = R0 * R0;
	return R0 + (1 - R0) * pow((1 - Cosine), 5);
}

static bool Refract(const float3& V, const float3& N, float NiOverNt, float3& Refracted)
{
	float3 UV = UnitVector(V);
	float DT = dot(UV, N);
	float Discriminant = 1.0f - NiOverNt * NiOverNt*(1.0f - DT * DT);

	if (Discriminant > 0)
	{
		Refracted = NiOverNt * (UV - N * DT) - N * sqrt(Discriminant);
		return true;
	}
	else
	{
		return false;
	}
}

bool MaterialScatter(const Material* Mat, const Ray& InRay, const FHit& Hit, float3& Attenuation, Ray& Scattered)
{
	if (Mat->Type == Lambertian)
	{
		float3 Target = Hit.P + Hit.Normal + RandomInUnitSphere();
		Scattered = MakeRay(Hit.P, Target - Hit.P, InRay.Time);
		Attenuation = Sample(&Mat->Albedo, Hit.P);
		return true;
	}
	else if (Mat->Type == Metal)
	{
		float3 Reflected = Reflect(UnitVector(InRay.Direction), Hit.Normal);
		Scattered = MakeRay(Hit.P, Reflected + Mat->Roughness * RandomInUnitSphere(), InRay.Time);
		Attenuation = Sample(&Mat->Albedo, Hit.P);
		return dot(Scattered.Direction, Hit.Normal) > 0;
	}
	else if (Mat->Type == Dielectric)
	{
		float3 OutwardNormal;
		float3 Reflected = Reflect(InRay.Direction, Hit.Normal);
		float NiOverNt;
		Attenuation = assign(1.0f, 1.0f, 1.0f);
		float3 Refracted;
		float ReflectProbability;
		float Cosine;

		float RayDirectionDotNormal = dot(InRay.Direction, Hit.Normal);

		if (RayDirectionDotNormal > 0)
		{
			OutwardNormal = assign(-Hit.Normal.e[0], -Hit.Normal.e[1], -Hit.Normal.e[2]);
			NiOverNt = Mat->Ri;
			Cosine = Mat->Ri * RayDirectionDotNormal / Length(InRay.Direction);
		}
		else
		{
			OutwardNormal = Hit.Normal;
			NiOverNt = 1.0f / Mat->Ri;
			Cosine = -RayDirectionDotNormal / Length(InRay.Direction);
		}

		if (Refract(InRay.Direction, OutwardNormal, NiOverNt, Refracted))
		{
			ReflectProbability = Schlick(Cosine, Mat->Ri);
		}
		else
		{
			ReflectProbability = 1.0f;
		}

		if (drand48() < ReflectProbability)
		{
			Scattered = MakeRay(Hit.P, Reflected, InRay.Time);
		}
		else
		{
			Scattered = MakeRay(Hit.P, Refracted, InRay.Time);
		}

		return true;
	}

	return false;
}

export void GetPixel(uniform ISPCBVHNode &RootNode, uniform Ray Rays[], uniform float3 Pixels[], uniform int Samples, uniform int i, uniform int Width, uniform int j, uniform int Height)
{
	seed_rng(&rngstate, programIndex + (i << (programIndex & 15)));

	const float3 StartColor = assign(1.0f, 1.0f, 1.0f);
	const float3 EndColor = assign(0.5f, 0.7f, 1.0f);

	foreach(s = 0 ... Samples)
	{
		float3 PixelColor = assign(0.0f, 0.0f, 0.0f);
		FHit Hit;

		Ray R = MakeRay(Rays[s].Origin, Rays[s].Direction, Rays[s].Time);

		if (Traverse(&RootNode, R, 0.001f, FLT_MAX, Hit))
		{
			float3 Attenuation;
			Ray Scattered;

			varying Ray SubmitRay = R;
			float3 Sum = assign(1,1,1);

			int Bounces = 0;

			while (Bounces < MAX_BOUNCES && MaterialScatter(Hit.Mat, SubmitRay, Hit, Attenuation, Scattered))
			{
				Sum = Sum * Attenuation;

				if (!Traverse(&RootNode, Scattered, 0.001f, FLT_MAX, Hit))
				{
					float3 UnitDirection = UnitVector(Scattered.Direction);
					float t = 0.5f * (UnitDirection.e[1] + 1.0f);
					Sum = Sum * (1.0f - t) * StartColor + t * EndColor;
					PixelColor = PixelColor + Sum;
					break;
				}

				SubmitRay = Scattered;
				Bounces += 1;
			}
		}
		else
		{
			float3 UnitDirection = UnitVector(R.Direction);
			float t = 0.5f * (UnitDirection.e[1] + 1.0f);
			PixelColor = PixelColor + (1.0f - t) * StartColor + t * EndColor;
		}

		Pixels[s] = PixelColor;

		print("PixelColor= % % %\n", PixelColor.e[0], PixelColor.e[1], PixelColor.e[2]);
	}
}
