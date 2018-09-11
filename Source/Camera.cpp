#include "Camera.h"
#include "utils/Random.h"

static FVector RandomInUnitDisk()
{
	FVector P;

	do 
	{
		P = 2.0f*FVector(float(Random::drand48()), float(Random::drand48()), 0) - FVector(1, 1, 0);
	} while (Dot(P,P) >= 1.0f);

	return P;
}

FCamera::FCamera(
	FVector LookFrom,
	FVector LookAt,
	FVector VUp,
	float VFov,
	float AspectRatio,
	float Aperture,
	float FocusDistance,
	float Begin,
	float End)
{
	BeginTime = Begin;
	EndTime = End;
	LensRadius = Aperture / 2;
	float Theta = float(VFov*M_PI / 180);
	float HalfHeight = tanf(Theta / 2);
	float HalfWidth = AspectRatio * HalfHeight;
	Origin = LookFrom;
	W = UnitVector(LookFrom - LookAt);
	U = UnitVector(Cross(VUp, W));
	V = Cross(W, U);
	LowerLeft = Origin - HalfWidth * FocusDistance * U - HalfHeight * FocusDistance * V - FocusDistance * W;
	Horizontal = 2 * HalfWidth * FocusDistance * U;
	Vertical = 2 * HalfHeight * FocusDistance * V;
}

Ray FCamera::GetRay(float s, float t)
{
	FVector RandomPointOnLens = LensRadius * RandomInUnitDisk();
	FVector Offset = U * RandomPointOnLens.x + V * RandomPointOnLens.y;
	float Time = float(BeginTime + Random::drand48()*(EndTime - BeginTime));
	return Ray(Origin + Offset, LowerLeft + s * Horizontal + t * Vertical - Origin - Offset, Time);
}

ispc::FCamera FCamera::GetISPCCamera()
{
	ispc::FCamera Camera;

	Camera.Origin = Origin.ToISPCFVector();
	Camera.LowerLeft = LowerLeft.ToISPCFVector();
	Camera.Horizontal = Horizontal.ToISPCFVector();
	Camera.Vertical = Vertical.ToISPCFVector();
	Camera.U = U.ToISPCFVector();
	Camera.V = V.ToISPCFVector();
	Camera.W = W.ToISPCFVector();
	Camera.LensRadius = LensRadius;
	Camera.BeginTime = BeginTime;
	Camera.EndTime = EndTime;

	return Camera;
}
