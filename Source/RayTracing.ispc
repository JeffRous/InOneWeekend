//typedef float<3> float3;

struct float3
{
	float e[3];
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
		P.e[0] = (2.0f * frandom(&rngstate)) - 1.0f;
		P.e[1] = (2.0f * frandom(&rngstate)) - 1.0f;
		P.e[2] = 0.0f;
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
		float3 Offset = add(mul(Camera.U, RandomPointOnLens.e[0]), mul(Camera.V, RandomPointOnLens.e[1]));

		Rays[s].Origin = add(Camera.Origin, Offset);
		Rays[s].Direction = sub(sub(add(add(mul(Camera.Horizontal, u), Camera.LowerLeft), mul(Camera.Vertical, v)), Camera.Origin), Offset);
		Rays[s].Time = Camera.BeginTime + frandom(&rngstate)*(Camera.EndTime - Camera.BeginTime);
	}
}
