/*
	Ray tracing in one weekend project
*/

#include <ppl.h>
#include "utils/Utils.h"
#include "FVector.h"
#include "Ray.h"
#include "Object.h"
#include "Sphere.h"
#include "Camera.h"
#include "Material.h"

#define WIDTH 200
#define HEIGHT 100
#define SAMPLES 100
#define PIXEL_COMPONENTS 3

FVector Color(const Ray& R, IObject *World, int32 Depth)
{
	FVector StartColor(1.0f, 1.0f, 1.0f);
	FVector EndColor(0.5f, 0.7f, 1.0f);

	FHit Hit;
	if (World->Hit(R, 0.001f, FLT_MAX, Hit))
	{
		Ray Scattered;
		FVector Attenuation;

		if (Depth < 50 && Hit.Material->Scatter(R, Hit, Attenuation, Scattered))
		{
			return Attenuation * Color(Scattered, World, Depth + 1);
		}
		else
		{
			return FVector(0, 0, 0);
		}
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

	IObject *List[5];
	List[0] = new Sphere(FVector(0.0f, 0.0f, -1.0f), 0.5f, new Lambertian(FVector(0.1f,0.2f,0.5f)));
	List[1] = new Sphere(FVector(0.0f, -100.5f, -1.0f), 100.0f, new Lambertian(FVector(0.8f,0.8f,0.0f)));
	List[2] = new Sphere(FVector(1.0f, 0.0f, -1.0f), 0.5f, new Metal(FVector(0.8f, 0.6f, 0.2f), 0.3f));
	List[3] = new Sphere(FVector(-1.0f, 0.0f, -1.0f), 0.5f, new Dielectric(1.5f));
	List[4] = new Sphere(FVector(-1.0f, 0.0f, -1.0f), -0.45f, new Dielectric(1.5f));

	IObject *World = new ObjectList(List, 5);
	FCamera Camera;

	Timer t;
	t.Start();

	concurrency::parallel_for(int32(0), HEIGHT, [&](int32 j)
	//for (int32 j = 0; j < HEIGHT; j++)
	{
		for (int32 i = 0; i < WIDTH; i++)
		{
			FVector PixelColor(0, 0, 0);

			for (int32 s = 0; s < SAMPLES; s++)
			{
				float u = float(i + Random::drand48()) / float(WIDTH);
				float v = float(j + Random::drand48()) / float(HEIGHT);

				Ray R = Camera.GetRay(u, v);
				FVector P = R.PointAtT(2.0);

				PixelColor += Color(R, World, 0);
			}

			PixelColor /= float(SAMPLES);
			PixelColor = FVector(sqrtf(PixelColor.r), sqrtf(PixelColor.g), sqrtf(PixelColor.b));

			uint32 PixelToWrite = ((HEIGHT-1-j)*WIDTH*PIXEL_COMPONENTS) + (i*PIXEL_COMPONENTS);

			ImageBuffer[PixelToWrite + 0] = uint8(255.99*PixelColor.r);
			ImageBuffer[PixelToWrite + 1] = uint8(255.99*PixelColor.g);
			ImageBuffer[PixelToWrite + 2] = uint8(255.99*PixelColor.b);
		}
	});

	double Elapsed = t.Stop();

	ImageFileWriter::WriteImage("output.png", ImageWriterType::PNG, WIDTH, HEIGHT, PIXEL_COMPONENTS, ImageBuffer);
	DebugPrint("Ray traversal time: %lf s \n", Elapsed/1000.0);

	delete ImageBuffer;
	ImageBuffer = nullptr;
	ImageBufferWriter = nullptr;

	return 0;
}
