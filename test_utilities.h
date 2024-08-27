#ifndef TEST_UTILITIES_H

#define COLOR_RED "\x1b[31m"
#define COLOR_GREEN "\x1b[32m"
#define COLOR_RESET "\x1b[0m "
#define COLOR_YELLOW "\x1b[33m"
#define COLOR_BLUE "\x1b[34m"
#define COLOR_MAGENTA "\x1b[35m"
#define COLOR_CYAN "\x1b[36m"

#define test_failed(fmt, args...)            \
	do {                                 \
		printf("%s", COLOR_RED);     \
		printf(fmt, ##args);         \
		printf("%s\n", COLOR_RESET); \
	} while (0);

#define test_passed(fmt, args...)            \
	do {                                 \
		printf("%s", COLOR_GREEN);   \
		printf(fmt, ##args);         \
		printf("%s\n", COLOR_RESET); \
	} while (0);

#define tests_partly_passed(fmt, args...)    \
	do {                                 \
		printf("%s", COLOR_YELLOW);  \
		printf(fmt, ##args);         \
		printf("%s\n", COLOR_RESET); \
	} while (0);

#endif // !TEST_UTILITIES_H
