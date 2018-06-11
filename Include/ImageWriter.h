#pragma once

#include "Types.h"

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
