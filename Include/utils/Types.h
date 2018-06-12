#pragma once

#include <float.h>

typedef unsigned char		uint8;
typedef signed char			int8;
static_assert(sizeof(char) == 1, "Char not 1 byte");

typedef unsigned short		uint16;
typedef signed short		int16;
static_assert(sizeof(short) == 2, "Short not 2 bytes");

typedef unsigned int		uint32;
typedef signed int			int32;
static_assert(sizeof(int) == 4, "Int not 4 bytes");

typedef unsigned long long	uint64;
typedef signed long	long	int64;
static_assert(sizeof(long long) == 8, "Long not 8 bytes");
