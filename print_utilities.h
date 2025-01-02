#ifndef PRINT_UTILITIES_H
#define PRINT_UTILITIES_H

#include <stdio.h>

#ifdef DEBUG
#define d_printf(fmt, args...) \
	printf("DEBUG: %s:%d: " fmt, __FILE__, __LINE__, ##args)
#define d_wprintf(fmt, args...) \
	wprintf("DEBUG: %s:%d: " fmt, __FILE__, __LINE__, ##args)
#else
#define d_printf(fmt, args...)
#define d_wprintf(fmt, args...)
#endif // !DEBUG

#define COLOR_RED "\x1b[31m"
#define COLOR_GREEN "\x1b[32m"
#define COLOR_RESET "\x1b[0m "
#define COLOR_YELLOW "\x1b[33m"
#define COLOR_BLUE "\x1b[34m"
#define COLOR_MAGENTA "\x1b[35m"
#define COLOR_CYAN "\x1b[36m"

#define print_red(fmt, args...)            \
	do {                                 \
		printf("%s", COLOR_RED);     \
		printf(fmt, ##args);         \
		printf("%s\n", COLOR_RESET); \
	} while (0);

#define print_green(fmt, args...)            \
	do {                                 \
		printf("%s", COLOR_GREEN);   \
		printf(fmt, ##args);         \
		printf("%s\n", COLOR_RESET); \
	} while (0);

#define print_yellow(fmt, args...)    \
	do {                                 \
		printf("%s", COLOR_YELLOW);  \
		printf(fmt, ##args);         \
		printf("%s\n", COLOR_RESET); \
	} while (0);

#define print_magenta(fmt, args...)    \
	do {                                 \
		printf("%s", COLOR_MAGENTA);  \
		printf(fmt, ##args);         \
		printf("%s\n", COLOR_RESET); \
	} while (0);

#define print_cyan(fmt, args...)    \
	do {                                 \
		printf("%s", COLOR_CYAN);  \
		printf(fmt, ##args);         \
		printf("%s\n", COLOR_RESET); \
	} while (0);

#endif // !PRINT_UTILITIES_H
