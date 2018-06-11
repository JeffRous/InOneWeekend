/*
	Ray tracing in one weekend project
*/

#include <stdio.h>
#include "Types.h"
#include "ImageWriter.h"
#include "FVector.h"
#include "Ray.h"
#include "Timer.h"
#include "DebugPrint.h"

#define WIDTH 200
#define HEIGHT 100
#define PIXEL_COMPONENTS 3

float HitSphere(const FVector &Center, float Radius, const Ray& R)
{
	FVector Oc = R.GetOrigin() - Center;
	float a = Dot(R.GetDirection(), R.GetDirection());
	float b = 2.0f * Dot(Oc, R.GetDirection());
	float c = Dot(Oc, Oc) - Radius * Radius;
	float Discriminant = b * b - 4 * a*c;
	if (Discriminant < 0)
	{
		return -1.0f;
	}
	else
	{
		return float((-b - sqrt(Discriminant)) / (2.0*a));
	}
}

FVector Color(const Ray& R)
{
	FVector StartColor(1.0f, 1.0f, 1.0f);
	FVector EndColor(0.5f, 0.7f, 1.0f);

	float t = HitSphere(FVector(0, 0, -1), 0.5, R);

	if (t > 0.0f)
	{
		FVector N = UnitVector(R.PointAt(t) - FVector(0, 0, -1));
		return 0.5 * FVector(N.x + 1, N.y + 1, N.z + 1);
	}
	else
	{
		FVector UnitDirection = UnitVector(R.GetDirection());
		float t = 0.5f * (UnitDirection.y + 1.0f);
		return (1.0f - t) * StartColor + t * EndColor;
	}
}

int main()
{
	InitTiming();

	uint8 ImageBuffer[WIDTH * HEIGHT * PIXEL_COMPONENTS];
	uint8 *ImageBufferWriter = ImageBuffer;

	FVector LowerLeft(-2.0f, -1.0f, -1.0f);
	FVector Horizontal(4.0f, 0.0f, 0.0f);
	FVector Vertical(0.0f, 2.0f, 0.0f);

	FVector Origin(0.0f, 0.0f, 0.0f);

	Timer t;
	t.Start();

	for (int j = HEIGHT-1; j >= 0; j--)
	{
		for (int i = 0; i < WIDTH; i++)
		{
			float u = float(i) / float(WIDTH);
			float v = float(j) / float(HEIGHT);

			Ray r(Origin, LowerLeft + u * Horizontal + v * Vertical);
			FVector col = Color(r);

			(*ImageBufferWriter++) = uint8(255.99*col.r);
			(*ImageBufferWriter++) = uint8(255.99*col.g);
			(*ImageBufferWriter++) = uint8(255.99*col.b);
		}
	}

	double Elapsed = t.Stop();

	ImageFileWriter::WriteImage("output.png", ImageWriterType::PNG, WIDTH, HEIGHT, PIXEL_COMPONENTS, ImageBuffer);
	DebugPrint("Ray traversal time: %lf ms \n", Elapsed);

	return 0;
}
