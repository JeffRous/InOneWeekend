#pragma once

#include <stdlib.h>

double drand48()
{
	return (double(rand())) / RAND_MAX;
}
