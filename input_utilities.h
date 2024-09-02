#ifndef INPUT_UTILITIES_H

#include <stdio.h>
#include <wchar.h>

int read_last_n(FILE *in, wchar_t *out, int n);
int read_first_n(FILE *in, char *out, int n);
int read_first_n_wide(FILE *in, wchar_t *out, int n);

#endif // INPUT_UTILITIES_H
