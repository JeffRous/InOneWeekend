#pragma once

#include <stdlib.h>

class Random
{
public:
	static double drand48() { return (double(rand())) / RAND_MAX; }
};
