/*
	Ray tracing in one weekend project
*/

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"

#define WIDTH 200
#define HEIGHT 100
#define PIXEL_COMPONENTS 3

int main()
{
	unsigned char ImageBuffer[WIDTH * HEIGHT * PIXEL_COMPONENTS];
	unsigned char *ImageBufferWriter = ImageBuffer;

	for (int j = HEIGHT-1; j >= 0; j--)
	{
		for (int i = 0; i < WIDTH; i++)
		{
			float r = float(i) / float(WIDTH);
			float g = float(j) / float(HEIGHT);
			float b = 0.2f;

			unsigned char cr = unsigned char(255.99*r);
			unsigned char cg = unsigned char(255.99*g);
			unsigned char cb = unsigned char(255.99*b);

			(*ImageBufferWriter++) = cr;
			(*ImageBufferWriter++) = cg;
			(*ImageBufferWriter++) = cb;
		}
	}

	stbi_write_png("output.png", WIDTH, HEIGHT, PIXEL_COMPONENTS, ImageBuffer, 0);
	//stbi_write_bmp("output.bmp", WIDTH, HEIGHT, PIXEL_COMPONENTS, ImageBuffer);
	//stbi_write_jpg("output.jpg", WIDTH, HEIGHT, PIXEL_COMPONENTS, ImageBuffer, 100);

	return 0;
}