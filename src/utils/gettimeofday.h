#pragma once

#include "utils/standard.h"

#ifdef WIN32
	extern "C"
	{ 
		int gettimeofday(struct timeval *tv, struct timezone *tz);
	}
#endif
