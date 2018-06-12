/*
	Ray tracing in one weekend project
*/

#include "utils/Utils.h"
#include "FVector.h"
#include "Ray.h"
#include "Object.h"
#include "Sphere.h"
#include "Camera.h"

#define WIDTH 200
#define HEIGHT 100
#define SAMPLES 100
#define PIXEL_COMPONENTS 3

FVector RandomInUnitSphere()
{
	FVector P;

	do
	{
		P = 2.0f*FVector(float(drand48()), float(drand48()), float(drand48())) - FVector(1, 1, 1);
	} while (P.SquaredLength() >= 1.0f);

	return P;
}

FVector Color(const Ray& R, IObject *World)
{
	FVector StartColor(1.0f, 1.0f, 1.0f);
	FVector EndColor(0.5f, 0.7f, 1.0f);

	FHit Hit;
	if (World->Hit(R, 0.001f, FLT_MAX, Hit))
	{
		FVector Target = Hit.P + Hit.Normal + RandomInUnitSphere();
		return 0.5*Color(Ray(Hit.P, Target - Hit.P), World);
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

	uint8 *ImageBuffer = new uint8[WIDTH * HEIGHT * PIXEL_COMPONENTS];
	uint8 *ImageBufferWriter = ImageBuffer;

	IObject *List[2];
	List[0] = new Sphere(FVector(0, 0, -1), 0.5);
	List[1] = new Sphere(FVector(0, -100.5, -1), 100);

	IObject *World = new ObjectList(List, 2);
	FCamera Camera;

	Timer t;
	t.Start();

	for (int32 j = HEIGHT-1; j >= 0; j--)
	{
		for (int32 i = 0; i < WIDTH; i++)
		{
			FVector PixelColor(0, 0, 0);

			for (int32 s = 0; s < SAMPLES; s++)
			{
				float u = float(i + drand48()) / float(WIDTH);
				float v = float(j + drand48()) / float(HEIGHT);

				Ray R = Camera.GetRay(u, v);
				FVector P = R.PointAtT(2.0);

				PixelColor += Color(R, World);
			}

			PixelColor /= float(SAMPLES);
			PixelColor = FVector(sqrtf(PixelColor.r), sqrtf(PixelColor.g), sqrtf(PixelColor.b));

			(*ImageBufferWriter++) = uint8(255.99*PixelColor.r);
			(*ImageBufferWriter++) = uint8(255.99*PixelColor.g);
			(*ImageBufferWriter++) = uint8(255.99*PixelColor.b);
		}
	}

	double Elapsed = t.Stop();

	ImageFileWriter::WriteImage("output.png", ImageWriterType::PNG, WIDTH, HEIGHT, PIXEL_COMPONENTS, ImageBuffer);
	DebugPrint("Ray traversal time: %lf ms \n", Elapsed);

	delete ImageBuffer;
	ImageBuffer = nullptr;
	ImageBufferWriter = nullptr;

	return 0;
}
