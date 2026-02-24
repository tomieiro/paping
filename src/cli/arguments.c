#include "cli/arguments.h"

#include "cli/i18n.h"

static int arguments_match(int pos, int total, pc_t argv[], pcc_t short_name, pcc_t long_name, int expected_value, int *value, int *any_match);

void arguments_print_banner(void)
{
	printf("paping v%s - Copyright (c) %d %s\n\n", VERSION, (int)YEAR, AUTHOR);
}

void arguments_print_usage(void)
{
	printf("%s\n", i18n_get_string(STRING_USAGE));
}

int arguments_process(int argc, pc_t argv[], arguments_t *arguments)
{
	int result;
	int value;
	int any_match;
	int got_destination;
	int i;

	if (arguments == NULL) return ERROR_INVALIDARGUMENTS;

	result = 0;
	value = 0;
	any_match = 0;
	got_destination = 0;

	arguments->Destination = NULL;
	arguments->Port = 0;
	arguments->Count = 0;
	arguments->Timeout = 1000;
	arguments->Type = IPPROTO_TCP;
	arguments->Continous = 1;
	arguments->UseColor = 1;
	arguments->ShowHelp = 0;

	for (i = 1; i < argc; i++)
	{
		any_match = 0;

		result = arguments_match(i, argc, argv, "-?", "--help", 0, &value, &any_match);
		if (result != SUCCESS) return result;
		if (!any_match)
		{
			result = arguments_match(i, argc, argv, "-h", "--help", 0, &value, &any_match);
			if (result != SUCCESS) return result;
		}

		if (any_match && value == 1)
		{
			arguments->ShowHelp = 1;
			return SUCCESS;
		}

		result = arguments_match(i, argc, argv, "-p", "--port", 1, &arguments->Port, &any_match);
		if (result != SUCCESS) return result;
		result = arguments_match(i, argc, argv, "-c", "--count", 1, &arguments->Count, &any_match);
		if (result != SUCCESS) return result;
		result = arguments_match(i, argc, argv, "-t", "--timeout", 1, &arguments->Timeout, &any_match);
		if (result != SUCCESS) return result;

		if (any_match)
		{
			i++;
			continue;
		}

		result = arguments_match(i, argc, argv, NULL, "--nocolor", 0, &value, &any_match);
		if (result != SUCCESS) return result;
		if (value == 1) arguments->UseColor = 0;

		if (!any_match)
		{
			if (!got_destination)
			{
				got_destination = 1;
				arguments->Destination = argv[i];
			}
			else
			{
				return ERROR_INVALIDARGUMENTS;
			}
		}
	}

	if (arguments->Count <= 0)
	{
		arguments->Count = -1;
		arguments->Continous = 1;
	}
	else
	{
		arguments->Continous = 0;
	}

	if (arguments->Port <= 0) return ERROR_INVALIDARGUMENTS;
	if (arguments->Timeout <= 0) return ERROR_INVALIDARGUMENTS;
	if (!got_destination) return ERROR_INVALIDARGUMENTS;

	return SUCCESS;
}

static int arguments_match(int pos, int total, pc_t argv[], pcc_t short_name, pcc_t long_name, int expected_value, int *value, int *any_match)
{
	if (any_match == NULL || value == NULL) return ERROR_INVALIDARGUMENTS;

	if (
		(short_name != NULL && strlen(short_name) == strlen(argv[pos]) && strcmp(short_name, argv[pos]) == 0) ||
		(long_name != NULL && strlen(long_name) == strlen(argv[pos]) && strcmp(long_name, argv[pos]) == 0))
	{
		*any_match = 1;

		if (expected_value)
		{
			if (pos + 1 == total) return ERROR_INVALIDARGUMENTS;
			*value = atoi(argv[pos + 1]);
		}
		else
		{
			*value = 1;
		}

		return SUCCESS;
	}

	if (!expected_value) *value = 0;

	return SUCCESS;
}

