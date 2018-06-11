#pragma once

#include <math.h>
#include "Types.h"

class FVector
{
public:
	FVector() {}
	FVector(float e0, float e1, float e2) { e[0] = e0; e[1] = e1; e[2] = e2; }

	inline const FVector& operator+() const { return *this; }
	inline FVector operator-() const { return FVector(-e[0], -e[1], -e[2]); }
	inline float operator[](int i) const { return e[i]; }
	inline float& operator[](int i) { return e[i]; }

	inline FVector& operator+=(const FVector &v2);
	inline FVector& operator-=(const FVector &v2);
	inline FVector& operator*=(const FVector &v2);
	inline FVector& operator/=(const FVector &v2);
	inline FVector& operator*=(const float t);
	inline FVector& operator/=(const float t);

	inline float Length() const { return sqrtf(e[0] * e[0] + e[1] * e[1] + e[2] * e[2]); }
	inline float SquaredLength() const { return e[0] * e[0] + e[1] * e[1] + e[2] * e[2]; }
	inline void Normalize();

	union
	{
		struct
		{
			float x, y, z;
		};

		struct
		{
			float r, g, b;
		};

		float e[3];
	};
};

inline void FVector::Normalize()
{
	float k = 1.0f / Length();
	e[0] *= k;
	e[1] *= k;
	e[2] *= k;
}

inline FVector operator+(const FVector &v1, const FVector &v2)
{
	return FVector(v1.e[0] + v2.e[0], v1.e[1] + v2.e[1], v1.e[2] + v2.e[2]);
}

inline FVector operator-(const FVector &v1, const FVector &v2)
{
	return FVector(v1.e[0] - v2.e[0], v1.e[1] - v2.e[1], v1.e[2] - v2.e[2]);
}

inline FVector operator*(const FVector &v1, const FVector &v2)
{
	return FVector(v1.e[0] * v2.e[0], v1.e[1] * v2.e[1], v1.e[2] * v2.e[2]);
}

inline FVector operator/(const FVector &v1, const FVector &v2)
{
	return FVector(v1.e[0] / v2.e[0], v1.e[1] / v2.e[1], v1.e[2] / v2.e[2]);
}

inline FVector operator*(const FVector &v, float t)
{
	return FVector(v.e[0] * t, v.e[1] * t, v.e[2] * t);
}

inline FVector operator*(float t, const FVector &v)
{
	return FVector(v.e[0] * t, v.e[1] * t, v.e[2] * t);
}

inline FVector operator/(const FVector &v, float t)
{
	return FVector(v.e[0] / t, v.e[1] / t, v.e[2] / t);
}

inline float Dot(const FVector &v1, const FVector &v2)
{
	return v1.e[0] * v2.e[0] + v1.e[1] * v2.e[1] + v1.e[2] * v2.e[2];
}

inline FVector Cross(const FVector &v1, const FVector &v2)
{
	return FVector((v1.e[1] * v2.e[2] - v1.e[2] * v2.e[1]),
		(-(v1.e[0] * v2.e[2] - v1.e[2] * v2.e[0])),
		(v1.e[0] * v2.e[1] - v1.e[1] * v2.e[0]));
}

inline FVector& FVector::operator+=(const FVector &v)
{
	e[0] += v.e[0];
	e[1] += v.e[1];
	e[2] += v.e[2];
	return *this;
}

inline FVector& FVector::operator-=(const FVector &v)
{
	e[0] -= v.e[0];
	e[1] -= v.e[1];
	e[2] -= v.e[2];
	return *this;
}

inline FVector& FVector::operator*=(const FVector &v)
{
	e[0] *= v.e[0];
	e[1] *= v.e[1];
	e[2] *= v.e[2];
	return *this;
}

inline FVector& FVector::operator/=(const FVector &v)
{
	e[0] /= v.e[0];
	e[1] /= v.e[1];
	e[2] /= v.e[2];
	return *this;
}

inline FVector& FVector::operator*=(const float t)
{
	e[0] *= t;
	e[1] *= t;
	e[2] *= t;
	return *this;
}

inline FVector& FVector::operator/=(const float t)
{
	e[0] /= t;
	e[1] /= t;
	e[2] /= t;
	return *this;
}

inline FVector UnitVector(FVector v)
{
	return v / v.Length();
}
