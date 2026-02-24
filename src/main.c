#include "utils/standard.h"

#include "cli/arguments.h"
#include "cli/i18n.h"
#include "net/host.h"
#include "net/socket.h"
#include "utils/print.h"
#include "utils/stats.h"

static void print_error(int error);
static int print_connect_info(const host_t *host);
static int print_successful_connection(const host_t *host, double time_ms);
static int print_failed_connection(int error);
static int print_stats(void);

static void signal_handler(int id);

static int use_color = 0;
static int exit_code = 0;
static stats_t stats;

int main(int argc, char *argv[])
{
	host_t host;
	int result;
	arguments_t arguments;

	unsigned int i;
	double time_ms;

	memset(&host, 0, sizeof(host));
	memset(&arguments, 0, sizeof(arguments));
	memset(&stats, 0, sizeof(stats));

	stats.Attempts = 0;
	stats.Connects = 0;
	stats.Failures = 0;
	stats.Minimum = 0.0;
	stats.Maximum = 0.0;
	stats.Total = 0.0;

	arguments_print_banner();

	result = arguments_process(argc, argv, &arguments);
	if (result != SUCCESS)
	{
		arguments_print_usage();
		return ERROR_INVALIDARGUMENTS;
	}

	if (arguments.ShowHelp)
	{
		arguments_print_usage();
		return SUCCESS;
	}

	use_color = arguments.UseColor;

	result = socket_resolve(arguments.Destination, &host);
	if (result != SUCCESS)
	{
		print_error(result);
		return result;
	}

	result = socket_set_port_and_type(arguments.Port, arguments.Type, &host);
	if (result != SUCCESS)
	{
		print_error(result);
		return result;
	}

	result = print_connect_info(&host);
	if (result != SUCCESS)
	{
		print_error(result);
		return result;
	}

	signal(SIGINT, &signal_handler);

	i = 0;
	time_ms = 0.0;
	while (arguments.Continous || i < (unsigned int)arguments.Count)
	{
		result = socket_connect(&host, arguments.Timeout, &time_ms);

		stats.Attempts++;

		if (result == SUCCESS)
		{
			stats.Connects++;
			stats.Total += time_ms;
			stats_update_max_min(&stats, time_ms);

			(void)print_successful_connection(&host, time_ms);
		}
		else
		{
			exit_code = 1;
			stats.Failures++;

			(void)print_failed_connection(result);
		}

#ifdef WIN32 /* Windows cannot sleep to that accuracy (I think!) */
		if ((int)time_ms < 1000) Sleep((DWORD)(1000 - (int)time_ms));
#else
		if ((int)time_ms < 1000) usleep((unsigned int)((1000 - (int)time_ms) * 1000));
#endif

		i++;
	}

	(void)print_stats();
	return exit_code;
}

static void signal_handler(int id)
{
	switch (id)
	{
		case SIGINT:
			(void)print_stats();
			exit(exit_code);
			return;
	}
}

static void print_error(int error)
{
	if (use_color)
		print_formatted(PRINT_COLOR_RED, i18n_get_string(error));
	else
		print_formatted(0, i18n_get_string(error));

	putchar('\n');
}

static int print_connect_info(const host_t *host)
{
	char info[1024];
	int result;

	result = host_format_connect_info(host, info, (int)sizeof(info));
	if (result != SUCCESS) return result;

	if (use_color)
		print_formatted(PRINT_COLOR_YELLOW, info);
	else
		print_formatted(0, info);

	printf("\n\n");
	return SUCCESS;
}

static int print_successful_connection(const host_t *host, double time_ms)
{
	char data[1024];
	int result;

	result = host_format_successful_connection(host, time_ms, data, (int)sizeof(data));
	if (result != SUCCESS) return result;

	if (use_color)
		print_formatted(PRINT_COLOR_GREEN, data);
	else
		print_formatted(0, data);

	putchar('\n');
	return SUCCESS;
}

static int print_stats(void)
{
	char buf[2048];
	int result;

	result = stats_format(&stats, buf, (int)sizeof(buf));
	if (result != SUCCESS) return result;

	if (use_color)
		print_formatted(PRINT_COLOR_BLUE, buf);
	else
		print_formatted(0, buf);

	putchar('\n');
	return SUCCESS;
}

static int print_failed_connection(int error)
{
	if (use_color)
		print_formatted(PRINT_COLOR_RED, i18n_get_string(error));
	else
		print_formatted(0, i18n_get_string(error));

	putchar('\n');
	return SUCCESS;
}
