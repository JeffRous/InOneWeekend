#pragma once

#include <cstdlib>

class Random
{
public:

	static double drand48()
	{
		if (!isSeeded())
		{
			std::srand(1);
			isSeeded() = true;
		}
		return (double(std::rand())) / RAND_MAX;
	}

private:
	static bool& isSeeded()
	{
		static bool bSeeded = false;
		return bSeeded;
	}
};
