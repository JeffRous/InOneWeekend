/*
	Ray tracing in one weekend project
*/

#include "Types.h"
#include "ImageWriter.h"
#include "FVector.h"

#define WIDTH 200
#define HEIGHT 100
#define PIXEL_COMPONENTS 3

int main()
{
	uint8 ImageBuffer[WIDTH * HEIGHT * PIXEL_COMPONENTS];
	uint8 *ImageBufferWriter = ImageBuffer;

	for (int j = HEIGHT-1; j >= 0; j--)
	{
		for (int i = 0; i < WIDTH; i++)
		{
			FVector col(float(i) / float(WIDTH), float(j) / float(HEIGHT), 0.2f);

			(*ImageBufferWriter++) = uint8(255.99*col.r);
			(*ImageBufferWriter++) = uint8(255.99*col.g);
			(*ImageBufferWriter++) = uint8(255.99*col.b);
		}
	}

	ImageFileWriter::WriteImage("output.png", ImageWriterType::PNG, WIDTH, HEIGHT, PIXEL_COMPONENTS, ImageBuffer);

	return 0;
}
