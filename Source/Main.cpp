/*
	Ray tracing in one weekend project
*/

#include "Types.h"
#include "ImageWriter.h"
#include "FVector.h"
#include "Ray.h"

#define WIDTH 200
#define HEIGHT 100
#define PIXEL_COMPONENTS 3

int main()
{
	uint8 ImageBuffer[WIDTH * HEIGHT * PIXEL_COMPONENTS];
	uint8 *ImageBufferWriter = ImageBuffer;

	FVector StartColor(1.0f, 1.0f, 1.0f);
	FVector EndColor(0.5f, 0.7f, 1.0f);

	FVector LowerLeft(-2.0f, -1.0f, -1.0f);
	FVector Horizontal(4.0f, 0.0f, 0.0f);
	FVector Vertical(0.0f, 2.0f, 0.0f);

	FVector Origin(0.0f, 0.0f, 0.0f);

	for (int j = HEIGHT-1; j >= 0; j--)
	{
		for (int i = 0; i < WIDTH; i++)
		{
			float u = float(i) / float(WIDTH);
			float v = float(j) / float(HEIGHT);

			Ray r(Origin, LowerLeft + u * Horizontal + v * Vertical);
			FVector col = Lerp(r, StartColor, EndColor);

			(*ImageBufferWriter++) = uint8(255.99*col.r);
			(*ImageBufferWriter++) = uint8(255.99*col.g);
			(*ImageBufferWriter++) = uint8(255.99*col.b);
		}
	}

	ImageFileWriter::WriteImage("output.png", ImageWriterType::PNG, WIDTH, HEIGHT, PIXEL_COMPONENTS, ImageBuffer);

	return 0;
}
