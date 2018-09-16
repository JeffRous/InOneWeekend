//typedef float<3> float3;

#define FLT_MAX          3.402823466e+38F        // max value
#define MAX_BOUNCES 50
#define MAX_BVH_DEPTH 64

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
	Material *Mat;
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

inline uniform float3 assign(uniform float x, uniform float y, uniform float z)
{
	uniform float3 ret;
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

bool IntersectBox(const uniform BoundingBox* uniform B, const Ray& R, float TMin, float TMax)
{
	float3 InvD = 1.0f / R.Direction;
	float3 T0 = (B->Min - R.Origin) * InvD;
	float3 T1 = (B->Max - R.Origin) * InvD;

	if(InvD.e[0] < 0.0f)
	{
		swap(T0.e[0], T1.e[0]);
	}

	TMin = max(T0.e[0], TMin);
	TMax = min(T1.e[0], TMax);

	if(InvD.e[1] < 0.0f)
	{
		swap(T0.e[1], T1.e[1]);
	}

	TMin = max(T0.e[1], TMin);
	TMax = min(T1.e[1], TMax);

	if(InvD.e[2] < 0.0f)
	{
		swap(T0.e[2], T1.e[2]);
	}

	TMin = max(T0.e[2], TMin);
	TMax = min(T1.e[2], TMax);

	return (TMin <= TMax);
}

float3 Sample(const Texture* T, const float3& p)
{
	float3 RetVal;

	if (T->Type == Constant)
	{
		RetVal = T->Color;
	}
	else if (T->Type == Checker)
	{
		float Sines = sin(10 * p.e[0])*sin(10 * p.e[1])*sin(10 * p.e[2]);

		if (Sines < 0)
		{
			RetVal = T->ColorOdd;
		}
		else
		{
			RetVal = T->ColorEven;
		}
	}
	else
	{
		RetVal = assign(0,0,0);
	}

	return RetVal;
}

float3 GetCenterAt(ISPCBVHNode *Node, float Time)
{
	float3 RetVal;

	if (Node->Obj->Type == Sphere)
	{
		RetVal = Node->Obj->Center0;
	}
	else if (Node->Obj->Type == MovingSphere)
	{
		RetVal = Node->Obj->Center0 + ((Time - Node->Obj->Time0) / (Node->Obj->Time1 - Node->Obj->Time0)) * (Node->Obj->Center1 - Node->Obj->Center0);
	}
	else
	{
		RetVal = assign(0,0,0);
	}

	return RetVal;
}

bool RayIntersect(const Ray& R, ISPCBVHNode *Node, float TMin, float TMax, FHit& Hit)
{
	bool RetVal = false;

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

			RetVal = true;
		}

		if(!RetVal)
		{
			Temp = (-b + DiscSqrt) / a;

			if (Temp < TMax && Temp > TMin)
			{
				Hit.T = Temp;
				Hit.P = R.Origin + Temp * R.Direction;
				Hit.Normal = (Hit.P - GetCenterAtTime) / Node->Obj->Radius;
				Hit.Mat = &Node->Obj->Mat;

				RetVal = true;
			}
		}
	}

	return RetVal;
}

inline uniform ISPCBVHNode* uniform NearChild(uniform ISPCBVHNode * uniform Current)
{
	return Current->Left;
}

inline uniform ISPCBVHNode* uniform FarChild(uniform ISPCBVHNode * uniform Current)
{
	return Current->Right;
}

bool Traverse(uniform ISPCBVHNode* uniform RootNode, const Ray& R, float TMin, float TMax, FHit& Hit)
{
	bool bHit = false;
	// Follow ray through BVH nodes to find primitive intersections
	uniform int TodoOffset = 0;
	uniform ISPCBVHNode* uniform Todo[MAX_BVH_DEPTH];

	uniform ISPCBVHNode* uniform Current = RootNode;

	Hit.T = FLT_MAX;

	while (true)
	{
		// Check ray against BVH node

		const uniform BoundingBox* uniform B = &Current->Box;
		if (any(IntersectBox(B, R, TMin, TMax)))
		{
			if (Current->ObjectType != BVH)
			{
				FHit PreviousHit = Hit;
				bool bHitRay = RayIntersect(R, Current, TMin, TMax, Hit);

				if(bHitRay)
				{
					bHit = true;

					if(PreviousHit.T < Hit.T)
					{
						Hit = PreviousHit;
					}
				}

				if (TodoOffset == 0)
				{
					break;
				}

				Current = Todo[--TodoOffset];
			}
			else
			{
				// Put far BVH node on _todo_ stack, advance to near node
				Todo[TodoOffset++] = FarChild(Current);
				Current = NearChild(Current);
			}
		}
		else
		{
			if (TodoOffset == 0)
			{
				break;
			}
			Current = Todo[--TodoOffset];
		}
	}

	return bHit;
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
	bool RetVal = false;

	if (Mat->Type == Lambertian)
	{
		float3 Target = Hit.P + Hit.Normal + RandomInUnitSphere();
		Scattered = MakeRay(Hit.P, Target - Hit.P, InRay.Time);
		Attenuation = Sample(&Mat->Albedo, Hit.P);
		RetVal = true;
	}
	else if (Mat->Type == Metal)
	{
		float3 Reflected = Reflect(UnitVector(InRay.Direction), Hit.Normal);
		Scattered = MakeRay(Hit.P, Reflected + Mat->Roughness * RandomInUnitSphere(), InRay.Time);
		Attenuation = Sample(&Mat->Albedo, Hit.P);
		RetVal = dot(Scattered.Direction, Hit.Normal) > 0;
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

		RetVal = true;
	}

	return RetVal;
}

export void GetPixel(uniform ISPCBVHNode * uniform RootNode, uniform Ray Rays[], uniform float3 Pixels[], uniform int Samples, uniform int i, uniform int Width, uniform int j, uniform int Height)
{
	seed_rng(&rngstate, programIndex + (i << (programIndex & 15)));

	const uniform float3 StartColor = assign(1.0f, 1.0f, 1.0f);
	const uniform float3 EndColor = assign(0.5f, 0.7f, 1.0f);

	foreach(s = 0 ... Samples)
	{
		float3 PixelColor = assign(0.0f, 0.0f, 0.0f);
		FHit Hit;

		Ray R = Rays[s];

		if (Traverse(RootNode, R, 0.001f, FLT_MAX, Hit))
		{
			float3 Attenuation;
			Ray Scattered;

			varying Ray SubmitRay = R;
			float3 Sum = assign(1,1,1);

			uniform int Bounces = 0;

			while (Bounces < MAX_BOUNCES && MaterialScatter(Hit.Mat, SubmitRay, Hit, Attenuation, Scattered))
			{
				Sum = Sum * Attenuation;

				if (!Traverse(RootNode, Scattered, 0.001f, FLT_MAX, Hit))
				{
					float3 UnitDirection = UnitVector(Scattered.Direction);
					float t = 0.5f * (UnitDirection.e[1] + 1.0f);
					Sum = Sum * ((1.0f - t) * StartColor + t * EndColor);
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
			PixelColor = PixelColor + ((1.0f - t) * StartColor + t * EndColor);
		}

		Pixels[s] = PixelColor;
	}
}
