#include <stdio.h>
#include <string.h>
#include <wchar.h>

#include "input_utilities.h"
#include "debug_prints.h"

// Ehkä siellä on joku null terminator lopussa mitä ei lueta ulos sieltä
// stdinistä

int read_first_n_wide(FILE *in, wchar_t *out, int n)
{
	fgetws(out, n, in);
        d_wprintf(L"%ls\n", out);
	int size = wcslen(out);
	/*if (size < n) {*/
	/*	out[size] = '\0';*/
        if (size >= n)
	{
                int next = fgetwc(in);
                while (next != EOF) {
                        next = fgetwc(in);
                }
        }
	return 0;
}

int read_first_n(FILE *in, char *out, int n)
{
	fgets(out, n, in);
	int size = strlen(out);
	/*if (size < n) {*/
	/*	out[size] = '\0';*/
        if (size >= n)
	{
                int next = fgetc(in);
                while (next != EOF) {
                        next = fgetc(in);
                }
        }
	return 0;
}

int read_last_n(FILE *in, wchar_t *out, int n)
{
	fgetws(out, n, in);
	int size = wcslen(out);
        if (size == 0) return -1;
        // note that size <= n - 1
	if (out[size] == '\n') {
		out[size] = '\0';
                return 0;
	} else {
                char next = fgetwc(in);
                while (next != '\n' || next != EOF) {
                        for (int i = 0; i < size - 1; i++) {
                                out[i] = out[i + 1];
                        }
                        out[size] = next;
                        next = fgetwc(in);
                }
        }
	return 0;
}
