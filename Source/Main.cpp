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
#include "BVH.h"
#include "Raytracing_ispc.h"

#define WIDTH 200
#define HEIGHT 100
#define SAMPLES 100
#define PIXEL_COMPONENTS 3
#define MAX_BOUNCES 50

static const bool bUseISPC = true;

IObject *TwoSpheres()
{
	ITexture *Checker = new CheckerTexture(new ConstantTexture(FVector(0.2f, 0.3f, 0.1f)), new ConstantTexture(FVector(0.9f, 0.9f, 0.9f)));

	int32 n = 2;
	IObject **List = new IObject*[n + 1];

	List[0] = new Sphere(FVector(0, -10, 0), 10, new Lambertian(Checker));
	List[1] = new Sphere(FVector(0, 10, 0), 10, new Lambertian(Checker));

	return new ObjectList(List, 2);
}

IObject *RandomWorld()
{
	int32 n = 50000;
	IObject **List = new IObject*[n+1];
	ITexture *Checker = new CheckerTexture(new ConstantTexture(FVector(0.2f, 0.3f, 0.1f)), new ConstantTexture(FVector(0.9f, 0.9f, 0.9f)));
	List[0] = new Sphere(FVector(0, -1000, 0), 1000, new Lambertian(Checker));

	int32 i = 1;

	for (int32 a = -10; a < 10; a++)
	{
		for (int32 b = -10; b < 10; b++)
		{
			float ChooseMaterial = float(Random::drand48());

			FVector Center(float(a + 0.9f*Random::drand48()), 0.2f, float(b + 0.9f*Random::drand48()));
			if ((Center - FVector(4, 0.2f, 0)).Length() > 0.9f)
			{
				if (ChooseMaterial < 0.8f) // diffuse
				{
					List[i++] = new MovingSphere(Center, Center+FVector(0,float(0.5f*Random::drand48()),0), 0.0f, 1.0f, 0.2f,
						new Lambertian(new ConstantTexture(FVector(float(Random::drand48()), float(Random::drand48()*Random::drand48()), float(Random::drand48()*Random::drand48())))));
				}
				else if (ChooseMaterial < 0.95f) // Metal
				{
					List[i++] = new Sphere(Center, 0.2f,
						new Metal(FVector(float(0.5f*(1 + Random::drand48())), float(0.5f*(1 + Random::drand48())), float(0.5f*(1 + Random::drand48()))), float(0.5f*Random::drand48())));
				}
				else // glass
				{
					List[i++] = new Sphere(Center, 0.2f, new Dielectric(1.5f));
				}
			}
		}
	}

	List[i++] = new Sphere(FVector(0, 1, 0), 1, new Dielectric(1.5f));
	List[i++] = new Sphere(FVector(-4, 1, 0), 1, new Lambertian(new ConstantTexture(FVector(0.4f, 0.2f, 0.1f))));
	List[i++] = new Sphere(FVector(4, 1, 0), 1, new Metal(FVector(0.7f, 0.6f, 0.5f), 0.0f));

	//return new ObjectList(List, i);
	return new BVHNode(List, i, 0, 1);
}

bool FindRayIntersection(const Ray& R, IObject *World, FHit& Hit)
{
	if (World->Hit(R, 0.001f, FLT_MAX, Hit))
	{
		return true;
	}
	else
	{
		return false;
	}
}

FVector GetPixel(IObject *World, ispc::Ray *Rays)
{
	FVector PixelColor(0, 0, 0);
	FVector StartColor(1.0f, 1.0f, 1.0f);
	FVector EndColor(0.5f, 0.7f, 1.0f);

	FHit Hit;

	for (int32 s = 0; s < SAMPLES; s++)
	{
		Ray R(Rays[s].Origin, Rays[s].Direction, Rays[s].Time);

		if (FindRayIntersection(R, World, Hit))
		{
			FVector Attenuation;
			Ray Scattered;

			Ray SubmitRay = R;
			FVector Sum(1, 1, 1);
			int32 Bounces = 0;

			while (Bounces < MAX_BOUNCES && Hit.Material->Scatter(SubmitRay, Hit, Attenuation, Scattered))
			{
				Sum *= Attenuation;

				if (!FindRayIntersection(Scattered, World, Hit))
				{
					FVector UnitDirection = UnitVector(Scattered.GetDirection());
					float t = 0.5f * (UnitDirection.y + 1.0f);
					Sum *= (1.0f - t) * StartColor + t * EndColor;
					PixelColor += Sum;
					break;
				}

				SubmitRay = Scattered;
				Bounces += 1;
			}
		}
		else
		{
			FVector UnitDirection = UnitVector(R.GetDirection());
			float t = 0.5f * (UnitDirection.y + 1.0f);
			PixelColor += (1.0f - t) * StartColor + t * EndColor;
		}
	}

	PixelColor /= float(SAMPLES);
	PixelColor = FVector(sqrtf(PixelColor.r), sqrtf(PixelColor.g), sqrtf(PixelColor.b));
	return PixelColor;
}

FVector Color(const Ray& R, IObject *World, int32 Bounces)
{
	FVector StartColor(1.0f, 1.0f, 1.0f);
	FVector EndColor(0.5f, 0.7f, 1.0f);

	FHit Hit;
	if (World->Hit(R, 0.001f, FLT_MAX, Hit))
	{
		Ray Scattered;
		FVector Attenuation;

		if (Bounces < MAX_BOUNCES && Hit.Material->Scatter(R, Hit, Attenuation, Scattered))
		{
			return Attenuation * Color(Scattered, World, Bounces + 1);
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

	IObject *World = RandomWorld();
	World->Debug();

	FVector Origin(13, 2, 3);
	FVector LookAt(0, 0, 0);
	FVector Up(0, 1, 0);
	float Fov = 20;
	float AspectRatio = float(WIDTH) / float(HEIGHT);
	float DistanceToFocus = 10.0f;
	float Aperture = 0.0f;
	float BeginTime = 0.0f;
	float EndTime = 1.0f;
	FCamera Camera(Origin, LookAt, Up, Fov, AspectRatio, Aperture, DistanceToFocus, BeginTime, EndTime);

	Timer t;
	t.Start();

	if (bUseISPC)
	{
		concurrency::parallel_for(int32(0), HEIGHT, [&](int32 j)
		//for (int32 j = 0; j < HEIGHT; j++)
		{
			ispc::Ray Rays[SAMPLES];

			for (int32 i = 0; i < WIDTH; i++)
			{
				ispc::FCamera ISPCCamera = Camera.GetISPCCamera();

				ispc::GetRays(Rays, ISPCCamera, SAMPLES, i, WIDTH, j, HEIGHT);

				FVector PixelColor = GetPixel(World, Rays);

				uint32 PixelToWrite = ((HEIGHT - 1 - j)*WIDTH*PIXEL_COMPONENTS) + (i*PIXEL_COMPONENTS);

				ImageBuffer[PixelToWrite + 0] = uint8(255.99*PixelColor.r);
				ImageBuffer[PixelToWrite + 1] = uint8(255.99*PixelColor.g);
				ImageBuffer[PixelToWrite + 2] = uint8(255.99*PixelColor.b);
			}
		});
	}
	else
	{
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

				uint32 PixelToWrite = ((HEIGHT - 1 - j)*WIDTH*PIXEL_COMPONENTS) + (i*PIXEL_COMPONENTS);

				ImageBuffer[PixelToWrite + 0] = uint8(255.99*PixelColor.r);
				ImageBuffer[PixelToWrite + 1] = uint8(255.99*PixelColor.g);
				ImageBuffer[PixelToWrite + 2] = uint8(255.99*PixelColor.b);
			}
		});
	}

	double Elapsed = t.Stop();

	ImageFileWriter::WriteImage("output.png", ImageWriterType::PNG, WIDTH, HEIGHT, PIXEL_COMPONENTS, ImageBuffer);
	DebugPrint("Ray traversal time: %lf s \n", Elapsed/1000.0);

	delete World;

	delete ImageBuffer;
	ImageBuffer = nullptr;
	ImageBufferWriter = nullptr;

	return 0;
}
