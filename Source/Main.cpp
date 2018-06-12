/*
	Ray tracing in one weekend project
*/

#include "Types.h"
#include "ImageWriter.h"
#include "FVector.h"
#include "Ray.h"
#include "Timer.h"
#include "Object.h"
#include "Sphere.h"
#include "DebugPrint.h"

#define WIDTH 200
#define HEIGHT 100
#define PIXEL_COMPONENTS 3

FVector Color(const Ray& R, IObject *World)
{
	FVector StartColor(1.0f, 1.0f, 1.0f);
	FVector EndColor(0.5f, 0.7f, 1.0f);

	FHit Hit;
	if (World->Hit(R, 0.0, FLT_MAX, Hit))
	{
		return 0.5*FVector(Hit.Normal.x + 1, Hit.Normal.y + 1, Hit.Normal.z + 1);
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

	FVector LowerLeft(-2.0f, -1.0f, -1.0f);
	FVector Horizontal(4.0f, 0.0f, 0.0f);
	FVector Vertical(0.0f, 2.0f, 0.0f);

	FVector Origin(0.0f, 0.0f, 0.0f);

	IObject *List[2];
	List[0] = new Sphere(FVector(0, 0, -1), 0.5);
	List[1] = new Sphere(FVector(0, -100.5, -1), 100);

	IObject *World = new ObjectList(List, 2);

	Timer t;
	t.Start();

	for (int j = HEIGHT-1; j >= 0; j--)
	{
		for (int i = 0; i < WIDTH; i++)
		{
			float u = float(i) / float(WIDTH);
			float v = float(j) / float(HEIGHT);

			Ray R(Origin, LowerLeft + u * Horizontal + v * Vertical);
			FVector P = R.PointAtT(2.0);
			FVector PixelColor = Color(R, World);

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
