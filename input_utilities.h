#ifndef INPUT_UTILITIES_H

#include <stdio.h>
#include <wchar.h>

#define BUF_SIZE 32;

int read_last_n(FILE *in, char *out, int n, char *header);
int read_first_n(FILE *in, char *out, size_t n, char *header);
int read_first_n_wide(FILE *in, wchar_t *out, int n);

#endif // INPUT_UTILITIES_H
