//typedef float<3> float3;

struct float3
{
	float x, y, z;
};

static inline float dot(float3 v1, float3 v2)
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

static inline float3 mul(float3 v, float t)
{
	float3 ret;
	ret.x = v.x * t;
	ret.y = v.y * t;
	ret.z = v.z * t;
	return ret;
}

static inline float3 mul(float3 v1, float3 v2)
{
	float3 ret;
	ret.x = v1.x * v2.x;
	ret.y = v1.y * v2.y;
	ret.z = v1.z * v2.z;
	return ret;
}

static inline float3 add(float3 v, float t)
{
	float3 ret;
	ret.x = v.x + t;
	ret.y = v.y + t;
	ret.z = v.z + t;
	return ret;
}

static inline float3 add(float3 v1, float3 v2)
{
	float3 ret;
	ret.x = v1.x + v2.x;
	ret.y = v1.y + v2.y;
	ret.z = v1.z + v2.z;
	return ret;
}

static inline float3 sub(float3 v1, float3 v2)
{
	float3 ret;
	ret.x = v1.x - v2.x;
	ret.y = v1.y - v2.y;
	ret.z = v1.z - v2.z;
	return ret;
}

export void Random(uniform float Randoms[], uniform unsigned int NumRandoms)
{
	RNGState rngstate;
	seed_rng(&rngstate, programIndex + (NumRandoms << (programIndex & 15)));

	foreach(i = 0 ... NumRandoms)
	{
		Randoms[i] = frandom(&rngstate);
	}
}

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

static inline float3 RandomInUnitDisk(RNGState& rngstate)
{
	float3 P;

	do
	{
		P.x = (2.0f * frandom(&rngstate)) - 1.0f;
		P.y = (2.0f * frandom(&rngstate)) - 1.0f;
		P.z = 0.0f;
	} while (dot(P,P) >= 1.0f);

	return P;
}

export void GetRays(uniform Ray Rays[], uniform FCamera &Camera, uniform int Samples, uniform int i, uniform int Width, uniform int j, uniform int Height)
{
	RNGState rngstate;
	seed_rng(&rngstate, programIndex + (i << (programIndex & 15)));

	foreach(s = 0 ... Samples)
	{
		float u = (i + frandom(&rngstate)) / Width;
		float v = (j + frandom(&rngstate)) / Height;

		float3 RandomPointOnLens = mul(RandomInUnitDisk(rngstate), Camera.LensRadius);
		float3 Offset = add(mul(Camera.U, RandomPointOnLens.x), mul(Camera.V, RandomPointOnLens.y));

		Rays[s].Origin = add(Camera.Origin, Offset);
		Rays[s].Direction = sub(sub(add(add(mul(Camera.Horizontal, u), Camera.LowerLeft), mul(Camera.Vertical, v)), Camera.Origin), Offset);
		Rays[s].Time = Camera.BeginTime + frandom(&rngstate)*(Camera.EndTime - Camera.BeginTime);
	}
}
