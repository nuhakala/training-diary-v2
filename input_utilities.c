#include <stdio.h>
#include <stdlib.h>
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

int read_first_n(FILE *in, char *out, size_t n, char * header)
{
	char *buffer;
	size_t bufsize = n;
	size_t characters;

	buffer = (char *)malloc(n * sizeof(char));
	if( buffer == NULL)
	{
		perror("Unable to allocate buffer");
		exit(1);
	}

	printf("%s", header);
	characters = getline(&buffer, &bufsize, stdin);
	strncpy(out, buffer, n - 1);
	out[n] = '\0';

	free(buffer);
	return characters;
}

int read_last_n(FILE *in, char *out, int n, char * header)
{
	char *buffer;
	size_t bufsize = n;
	size_t characters;

	buffer = (char *)malloc(bufsize * sizeof(char));
	if( buffer == NULL)
	{
		perror("Unable to allocate buffer");
		exit(1);
	}

	printf("%s", header);
	characters = getline(&buffer, &bufsize, stdin);

	// calculate boundaries and copy the result
	int start = characters - n;
	if (start < 0) start = 0;
	int end = start + n;
	if (end > characters) end = characters - start - 1;
	strncpy(out, buffer + start, end);
	out[end] = '\0';

	free(buffer);
	return characters;
}
