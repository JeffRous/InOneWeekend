#include <stdio.h>
#include <stdarg.h>
#include <Windows.h>

void DebugPrint(const wchar_t *format, ...)
{
	wchar_t buffer[256];
	va_list args;
	va_start(args, format);
	vswprintf_s(buffer, format, args);
	wprintf(buffer);
	OutputDebugStringW(buffer);
	va_end(args);
}
