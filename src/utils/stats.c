#include "utils/stats.h"

#include "cli/i18n.h"

double stats_average(const stats_t *stats)
{
	if (stats == NULL) return 0.0;
	if (stats->Attempts <= 0) return 0.0;
	return stats->Total / (double)stats->Attempts;
}

void stats_update_max_min(stats_t *stats, double value)
{
	if (stats == NULL) return;

	if (stats->Attempts == 1)
	{
		stats->Minimum = value;
		stats->Maximum = value;
		return;
	}

	if (value < stats->Minimum) stats->Minimum = value;
	if (value > stats->Maximum) stats->Maximum = value;
}

int stats_format(const stats_t *stats, pc_t out, int out_len)
{
	pcc_t format;
	double fail_percent;

	if (stats == NULL || out == NULL || out_len <= 0) return ERROR_INVALIDARGUMENTS;

	format = i18n_get_string(STRING_STATS);
	fail_percent = 0.0;
	if (stats->Attempts > 0) fail_percent = ((double)stats->Failures / (double)stats->Attempts) * 100.0;

	snprintf(
		out,
		(size_t)out_len,
		format,
		stats->Attempts,
		stats->Connects,
		stats->Failures,
		fail_percent,
		stats->Minimum,
		stats->Maximum,
		stats_average(stats));

	out[out_len - 1] = '\0';
	return SUCCESS;
}

