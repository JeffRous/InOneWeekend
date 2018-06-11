#pragma once

#include <windows.h>
#include "Types.h"

static double Frequency;

void InitTiming()
{
	LARGE_INTEGER LiFrequency;
	QueryPerformanceFrequency(&LiFrequency);
	Frequency = double(LiFrequency.QuadPart) / 1000.0;
}

class Timer
{
public:
	void Start()
	{
		QueryPerformanceCounter(&StartTime);
	}

	double Stop()
	{
		LARGE_INTEGER FinishTime;
		QueryPerformanceCounter(&FinishTime);
		return (FinishTime.QuadPart - StartTime.QuadPart) / Frequency;
	}

private:
	LARGE_INTEGER StartTime;
};
