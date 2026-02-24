#ifndef PAPING_UTILS_STATS_H
#define PAPING_UTILS_STATS_H

#include "utils/standard.h"

typedef struct stats_s
{
	double Minimum;
	double Maximum;
	double Total;

	int Attempts;
	int Connects;
	int Failures;
} stats_t;

double stats_average(const stats_t *stats);
void stats_update_max_min(stats_t *stats, double value);
int stats_format(const stats_t *stats, pc_t out, int out_len);

#endif /* PAPING_UTILS_STATS_H */
