#pragma once

#include "Ray.h"

class FCamera
{
public:
	FCamera(FVector LookFrom, FVector LookAt, FVector VUp, float VFov, float AspectRatio, float Aperture, float FocusDistance);

	Ray GetRay(float s, float t);

private:
	FVector Origin;
	FVector LowerLeft;
	FVector Horizontal;
	FVector Vertical;
	FVector U, V, W;
	float LensRadius;
};
