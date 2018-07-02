#pragma once

#include "pcg-c-basic/pcg_basic.h"

#define RANDOM_BOUND 0x7FFF

class Random
{
public:

	static double drand48()
	{
		if (!isSeeded())
		{
			pcg32_srandom_r(GetGenerator(), 42u, 54u);
			isSeeded() = true;
		}
		return (double(pcg32_boundedrand_r(GetGenerator(), RANDOM_BOUND))) / RANDOM_BOUND;
	}

private:

	static pcg32_random_t* GetGenerator()
	{
		static pcg32_random_t Generator = { 0 };
		return &Generator;
	}

	static bool& isSeeded()
	{
		static bool bSeeded = false;
		return bSeeded;
	}
};
