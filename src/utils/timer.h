#ifndef PAPING_UTILS_TIMER_H
#define PAPING_UTILS_TIMER_H

#include "utils/standard.h"

typedef struct paping_timer_s
{
	int HasValue;
	struct timeval Start;
	struct timeval Stop;
} paping_timer_t;

void timer_start(paping_timer_t *timer);
double timer_stop(paping_timer_t *timer);

#endif /* PAPING_UTILS_TIMER_H */
