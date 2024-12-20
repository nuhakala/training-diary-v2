#ifndef INPUT_UTILITIES_H
#define INPUT_UTILITIES_H

#include <stdio.h>
#include <wchar.h>

#define BUF_SIZE 10;

int read_last_n(FILE *in, char **out, int n, char *header);
int read_first_n(FILE *in, char **out, int n, char *header);

char *read_int_input(char *header, int low, int up);
char *read_double_input(char *header, double low, double up);
char *read_string_input(char *header, int n);
char *read_char_input(char *header, const char *options);
char *read_time_input(char *header);

#endif // INPUT_UTILITIES_H
