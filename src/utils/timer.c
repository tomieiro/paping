#include "utils/timer.h"

#ifdef WIN32
#include "utils/gettimeofday.h"
#endif

void timer_start(paping_timer_t *timer)
{
	if (timer == NULL) return;

	timer->HasValue = 1;
	gettimeofday(&timer->Start, NULL);
}

double timer_stop(paping_timer_t *timer)
{
	double usec_stop;
	double usec_start;

	if (timer == NULL) return 0.0;
	if (!timer->HasValue) return 0.0;

	gettimeofday(&timer->Stop, NULL);

	usec_stop = (double)timer->Stop.tv_usec + ((double)timer->Stop.tv_sec * 1000000.0);
	usec_start = (double)timer->Start.tv_usec + ((double)timer->Start.tv_sec * 1000000.0);

	return (usec_stop - usec_start) / 1000.0;
}
