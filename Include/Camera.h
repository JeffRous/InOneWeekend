#pragma once

#include "Ray.h"
#include "Raytracing_ispc.h"

class FCamera
{
public:
	FCamera(
		FVector LookFrom,
		FVector LookAt,
		FVector VUp,
		float VFov,
		float AspectRatio,
		float Aperture,
		float FocusDistance,
		float BeginTime,
		float EndTime);

	Ray GetRay(float s, float t);
	ispc::FCamera GetISPCCamera();

private:
	FVector Origin;
	FVector LowerLeft;
	FVector Horizontal;
	FVector Vertical;
	FVector U, V, W;
	float LensRadius;
	float BeginTime, EndTime;
};
