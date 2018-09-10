#pragma once

static thread_local uint32 RndState = 1;

class Random
{
public:
	static float drand48()
	{
		return (XorShift32() & 0xFFFFFF) / 16777216.0f;
	}

private:
	static uint32 XorShift32()
	{
		uint32 x = RndState + 1;
		x ^= x << 13;
		x ^= x >> 17;
		x ^= x << 15;
		RndState = x;
		return x;
	}
};
