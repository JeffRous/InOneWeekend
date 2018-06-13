#pragma once

#include "utils/Utils.h"

class FCamera
{
public:
	FCamera(FVector LookFrom, FVector LookAt, FVector VUp, float VFov, float Aspect)
	{
		FVector U, V, W;
		float Theta = float(VFov*M_PI/180);
		float HalfHeight = tanf(Theta / 2);
		float HalfWidth = Aspect * HalfHeight;
		Origin = LookFrom;
		W = UnitVector(LookFrom - LookAt);
		U = UnitVector(Cross(VUp, W));
		V = Cross(W, U);
		LowerLeft = Origin - HalfWidth*U - HalfHeight*V - W;
		Horizontal = 2 * HalfWidth * U;
		Vertical = 2 * HalfHeight * V;
	}

	Ray GetRay(float u, float v) { return Ray(Origin, LowerLeft + u * Horizontal + v * Vertical - Origin); }

private:
	FVector LowerLeft;
	FVector Horizontal;
	FVector Vertical;
	FVector Origin;
};
