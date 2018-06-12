#pragma once

void InitTiming();

class Timer
{
public:
	void Start();
	double Stop();

private:
	double StartTime;
};
