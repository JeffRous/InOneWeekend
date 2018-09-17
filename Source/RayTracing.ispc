#define FLT_MAX          3.402823466e+38F        // max value
#define MAX_BOUNCES 50
#define MAX_BVH_DEPTH 64

static RNGState rngstate;

typedef float<3> float3;

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
	uniform float3 Color;

	uniform float3 ColorEven;
	uniform float3 ColorOdd;
	uniform ETextureType Type;
};

struct Material
{
	uniform Texture Albedo;
	uniform float Ri;
	uniform float Roughness;
	uniform EMaterialType Type;
};

struct Object
{
	uniform Material Mat;
	uniform float3 Center0, Center1;
	uniform float Time0, Time1;
	uniform float Radius;
	uniform EObjectType Type;
};

struct BoundingBox
{
	uniform float3 Min;
	uniform float3 Max;
};

struct ISPCBVHNode
{
	uniform BoundingBox Box;

	uniform EObjectType ObjectType;
	uniform Object *Obj;

	uniform ISPCBVHNode* Parent;
	uniform ISPCBVHNode* Left;
	uniform ISPCBVHNode* Right;
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
	uniform float3 Origin;
	uniform float3 LowerLeft;
	uniform float3 Horizontal;
	uniform float3 Vertical;
	uniform float3 U, V, W;
	uniform float LensRadius;
	uniform float BeginTime, EndTime;
};

struct Ray
{
	float3 Origin;
	float3 Direction;
	float Time;
};

static inline void swap(float& a, float& b)
{
	float t = a;
	a = b;
	b = t;
}

static inline float dot(const float3 &v1, const float3 &v2)
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

static inline float3 operator+(const float3 &v1, const float3 &v2)
{
	float3 ret;
	ret.x = v1.x + v2.x;
	ret.y = v1.y + v2.y;
	ret.z = v1.z + v2.z;
	return ret;
}

static inline float3 operator-(const float3 &v1, const float3 &v2)
{
	float3 ret;
	ret.x = v1.x - v2.x;
	ret.y = v1.y - v2.y;
	ret.z = v1.z - v2.z;
	return ret;
}

static inline float3 operator*(const float3 &v1, const float3 &v2)
{
	float3 ret;
	ret.x = v1.x * v2.x;
	ret.y = v1.y * v2.y;
	ret.z = v1.z * v2.z;
	return ret;
}

static inline float3 operator/(const float3 &v1, const float3 &v2)
{
	float3 ret;
	ret.x = v1.x / v2.x;
	ret.y = v1.y / v2.y;
	ret.z = v1.z / v2.z;
	return ret;
}

static inline float3 operator*(const float3 &v, const float t)
{
	float3 ret;
	ret.x = v.x * t;
	ret.y = v.y * t;
	ret.z = v.z * t;
	return ret;
}

static inline float3 operator*(const float t, const float3 &v)
{
	float3 ret;
	ret.x = v.x * t;
	ret.y = v.y * t;
	ret.z = v.z * t;
	return ret;
}

static inline float3 operator/(const float3 &v, const float t)
{
	float3 ret;
	ret.x = v.x / t;
	ret.y = v.y / t;
	ret.z = v.z / t;
	return ret;
}

static inline float3 operator/(const float t, const float3 &v)
{
	float3 ret;
	ret.x = t / v.x;
	ret.y = t / v.y;
	ret.z = t / v.z;
	return ret;
}

static inline float3 assign(const float x, const float y, const float z)
{
	float3 ret;
	ret.x = x;
	ret.y = y;
	ret.z = z;
	return ret;
}

static inline uniform float3 assign(const uniform float x, const uniform float y, const uniform float z)
{
	uniform float3 ret;
	ret.x = x;
	ret.y = y;
	ret.z = z;
	return ret;
}

static inline float Length(const float3 &v)
{
	return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

static inline float SquaredLength(const float3 &v)
{
	return v.x * v.x + v.y * v.y + v.z * v.z;
}

static inline float3 UnitVector(const float3 &v)
{
	return v / Length(v);
}

inline Ray MakeRay(const float3 &Origin, const float3 &Direction, const float Time)
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

static float3 RandomInUnitDisk()
{
	float3 P;

	do
	{
		P = 2.f*assign(drand48(), drand48(), 0.f) - assign(1.f, 1.f, 0.f);
	} while (dot(P,P) >= 1.f);

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

export void GetRays(uniform Ray Rays[], const uniform FCamera& Camera, uniform int Samples, uniform int i, uniform int Width, uniform int j, uniform int Height)
{
	seed_rng(&rngstate, programIndex + (i << (programIndex & 15)));

	foreach(s = 0 ... Samples)
	{
		float u = (i + drand48()) / Width;
		float v = (j + drand48()) / Height;

		float3 RandomPointOnLens = Camera.LensRadius * RandomInUnitDisk();
		float3 Offset = Camera.U * RandomPointOnLens.x + Camera.V * RandomPointOnLens.y;

		float3 Origin = Camera.Origin + Offset;
		float3 Direction = Camera.LowerLeft + (u * Camera.Horizontal) + (v * Camera.Vertical) - Camera.Origin - Offset;
		float Time = Camera.BeginTime + drand48()*(Camera.EndTime - Camera.BeginTime);

		Rays[s] = MakeRay(Origin, Direction, Time);
	}
}

bool IntersectBox(const uniform BoundingBox* uniform B, const Ray& R, float TMin, float TMax)
{
	float3 InvD = 1.0f / R.Direction;
	float3 T0 = (B->Min - R.Origin) * InvD;
	float3 T1 = (B->Max - R.Origin) * InvD;

	if(InvD.x < 0.0f)
	{
		swap(T0.x, T1.x);
	}

	TMin = max(T0.x, TMin);
	TMax = min(T1.x, TMax);

	if(InvD.y < 0.0f)
	{
		swap(T0.y, T1.y);
	}

	TMin = max(T0.y, TMin);
	TMax = min(T1.y, TMax);

	if(InvD.z < 0.0f)
	{
		swap(T0.z, T1.z);
	}

	TMin = max(T0.z, TMin);
	TMax = min(T1.z, TMax);

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
		float Sines = sin(10 * p.x)*sin(10 * p.y)*sin(10 * p.z);

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

float3 GetCenterAt(uniform ISPCBVHNode * uniform Node, float Time)
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

bool RayIntersect(const Ray& R, uniform ISPCBVHNode* uniform Node, float TMin, float TMax, FHit& Hit)
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
			OutwardNormal = Hit.Normal * -1.0f;
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

export void GetPixel(uniform ISPCBVHNode* uniform RootNode, const uniform FCamera &Camera, uniform float3& Pixel, uniform int Samples, uniform int i, uniform int Width, uniform int j, uniform int Height)
{
	seed_rng(&rngstate, programIndex + (i << (programIndex & 15)));

	const uniform float3 StartColor = assign(1.0f, 1.0f, 1.0f);
	const uniform float3 EndColor = assign(0.5f, 0.7f, 1.0f);
	float3 FinalColor = assign(0.0f, 0.0f, 0.0f);

	foreach(s = 0 ... Samples)
	{
		float u = (i + drand48()) / Width;
		float v = (j + drand48()) / Height;

		float3 RandomPointOnLens = RandomInUnitDisk() * Camera.LensRadius;
		float3 Offset = Camera.U * RandomPointOnLens.x + Camera.V * RandomPointOnLens.y;

		Ray R;
		R.Origin = Camera.Origin + Offset;
		R. Direction = Camera.LowerLeft + u * Camera.Horizontal + v * Camera.Vertical - Camera.Origin - Offset;
		R.Time = Camera.BeginTime + drand48()*(Camera.EndTime - Camera.BeginTime);

		float3 PixelColor = assign(0.0f, 0.0f, 0.0f);
		FHit Hit;

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
					float t = 0.5f * (UnitDirection.y + 1.0f);
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
			float t = 0.5f * (UnitDirection.y + 1.0f);
			PixelColor = PixelColor + ((1.0f - t) * StartColor + t * EndColor);
		}

		FinalColor = FinalColor + PixelColor;
	}

	Pixel = assign(reduce_add(FinalColor.x), reduce_add(FinalColor.y), reduce_add(FinalColor.z));
}
