#ifndef PAPING_UTILS_GETTIMEOFDAY_H
#define PAPING_UTILS_GETTIMEOFDAY_H

#include "utils/standard.h"

#ifdef WIN32
struct timezone;
int gettimeofday(struct timeval *tv, struct timezone *tz);
#endif

#endif /* PAPING_UTILS_GETTIMEOFDAY_H */
