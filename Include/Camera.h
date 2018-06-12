#pragma once

class FCamera
{
public:
	FCamera()
	{
		LowerLeft = FVector(-2.0f, -1.0f, -1.0f);
		Horizontal = FVector(4.0f, 0.0f, 0.0f);
		Vertical = FVector(0.0f, 2.0f, 0.0f);
		Origin = FVector(0.0f, 0.0f, 0.0f);
	}

	FCamera(FVector& InOrigin, FVector& InLowerLeft, FVector& InHorizontal, FVector& InVertical)
	{
		Origin = InOrigin;
		LowerLeft = InLowerLeft;
		Horizontal = InHorizontal;
		Vertical = InVertical;
	}

	Ray GetRay(float u, float v) { return Ray(Origin, LowerLeft + u * Horizontal + v * Vertical - Origin); }

private:
	FVector LowerLeft;
	FVector Horizontal;
	FVector Vertical;
	FVector Origin;
};
