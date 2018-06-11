#pragma once

#include "Types.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"

enum ImageWriterType
{
	PNG = 0,
	JPG,
	BMP
};

class ImageFileWriter
{
public:
	static bool WriteImage(const char *FileName, ImageWriterType FileType, int32 Width, int32 Height, int32 Components, uint8 *Data);
};

bool ImageFileWriter::WriteImage(const char *FileName, ImageWriterType FileType, int32 Width, int32 Height, int32 Components, uint8 *Data)
{
	int Result = 0;

	switch (FileType)
	{
	case PNG:
		Result = stbi_write_png(FileName, Width, Height, Components, Data, 0);
		break;
	case JPG:
		Result = stbi_write_jpg(FileName, Width, Height, Components, Data, 100);
		break;
	case BMP:
		Result = stbi_write_bmp(FileName, Width, Height, Components, Data);
		break;
	}

	return !!Result;
}