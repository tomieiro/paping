#ifndef PAPING_CLI_ARGUMENTS_H
#define PAPING_CLI_ARGUMENTS_H

#include "utils/standard.h"

typedef struct arguments_s
{
	pcc_t Destination;
	int Port;
	int Type;
	int Timeout;
	int Count;
	int Continous;
	int UseColor;
	int ShowHelp;
} arguments_t;

void arguments_print_banner(void);
void arguments_print_usage(void);
int arguments_process(int argc, pc_t argv[], arguments_t *arguments);

#endif /* PAPING_CLI_ARGUMENTS_H */
