#include "Timer.h"

#include <windows.h>
#include "Types.h"

static double Frequency;

void InitTiming()
{
	LARGE_INTEGER LiFrequency;
	QueryPerformanceFrequency(&LiFrequency);
	Frequency = double(LiFrequency.QuadPart) / 1000.0;
}

void Timer::Start()
{
	LARGE_INTEGER LiStart;
	QueryPerformanceCounter(&LiStart);

	StartTime = (double)LiStart.QuadPart;
}

double Timer::Stop()
{
	LARGE_INTEGER LiFinish;
	QueryPerformanceCounter(&LiFinish);
	return (LiFinish.QuadPart - StartTime) / Frequency;
}
