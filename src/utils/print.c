#include "utils/print.h"

#ifdef _WIN32
static int print_initial_colors = 0;
static int print_has_initial_colors = 0;
#endif

void print_formatted(int color, pcc_t data)
{
	int in_marker;
	const char *p;

	if (data == NULL) return;

	in_marker = 0;
	p = data;

	for (; *p != '\0'; p++)
	{
		if (*p == '$')
		{
			in_marker = !in_marker;
			if (color != 0)
			{
				if (in_marker) print_set_color(color);
				else print_reset_color();
			}
			continue;
		}

		putchar((unsigned char)*p);
	}

	if (color != 0) print_reset_color();
}

void print_set_color(int color)
{
#ifdef _WIN32
	HANDLE console;
	CONSOLE_SCREEN_BUFFER_INFO buffer_info;

	console = GetStdHandle(STD_OUTPUT_HANDLE);

	if (!print_has_initial_colors)
	{
		if (GetConsoleScreenBufferInfo(console, &buffer_info))
		{
			print_initial_colors = (int)buffer_info.wAttributes;
			print_has_initial_colors = 1;
		}
	}

	SetConsoleTextAttribute(console, (WORD)color);
#else
	printf("\033[01;%dm", color);
#endif
}

void print_reset_color(void)
{
#ifdef _WIN32
	if (print_has_initial_colors)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (WORD)print_initial_colors);
	}
#else
	printf("\033[22;%dm", 0);
#endif
}
