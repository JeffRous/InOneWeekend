/*
	Ray tracing in one weekend project
*/

#include "Types.h"
#include "ImageWriter.h"

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
			float r = float(i) / float(WIDTH);
			float g = float(j) / float(HEIGHT);
			float b = 0.2f;

			uint8 cr = uint8(255.99*r);
			uint8 cg = uint8(255.99*g);
			uint8 cb = uint8(255.99*b);

			(*ImageBufferWriter++) = cr;
			(*ImageBufferWriter++) = cg;
			(*ImageBufferWriter++) = cb;
		}
	}

	ImageFileWriter::WriteImage("output.png", ImageWriterType::PNG, WIDTH, HEIGHT, PIXEL_COMPONENTS, ImageBuffer);

	return 0;
}
