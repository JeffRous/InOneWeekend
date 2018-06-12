#include <stdio.h>
#include <stdarg.h>
#include <Windows.h>

void DebugPrint(const char *format, ...)
{
	char buffer[256];
	va_list args;
	va_start(args, format);
	vsprintf_s(buffer, format, args);
	OutputDebugString(buffer);
	va_end(args);
}
