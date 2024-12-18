#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#include "input_utilities.h"
#include "debug_prints.h"
#include "utf8.h"

char *read_char_input(char *header, char *options)
{
#pragma GCC diagnostic ignored "-Wuninitialized"
	char *p;
	while (1) {
		read_first_n(stdin, &p, 1, header);
		for (int i = 0; i < sizeof(options); i++) {
			if (*p == options[i])
				return p;
		}
	}
}

char *read_time_input(char *header)
{
	char *p;
	while (1) {
		read_first_n(stdin, &p, 10, header);
		int hours, seconds;
		if (sscanf(p, "%d.%d", &hours, &seconds) == 2)
			return p;
	}
}

char *read_string_input(char *header, int n)
{
	char *p;
	read_first_n(stdin, &p, n, header);
	return p;
}

char *read_int_input(char *header, int low, int up)
{
	char *p;
	while (1) {
		read_first_n(stdin, &p, 4, header);
		int num;
		if (sscanf(p, "%d", &num) < 1) continue;
		if (num >= low && num <= up)
			return p;
	}
}

char *read_double_input(char *header, double low, double up)
{
	char *p;
	while (1) {
		read_first_n(stdin, &p, 10, header);
		double num;
		if (sscanf(p, "%lf", &num) < 1) continue;
		if (num >= low && num <= up)
			return p;
	}
}

int read_first_n(FILE *in, char **out, int n, char *header)
{
	char *buffer;
	size_t bufsize = n + 1;
	buffer = (char *)malloc(4 * bufsize * sizeof(char));
	if (buffer == NULL) {
		perror("Unable to allocate buffer");
		exit(1);
	}

	printf("%s", header);
	getline(&buffer, &bufsize, stdin);
	int utf8_len = u8_strlen(buffer);
	int end = utf8_len < n ? utf8_len : n;
	int bytenum = u8_offset(buffer, end);
	buffer[bytenum] = '\0';
	*out = buffer;

	return utf8_len;
}

int read_last_n(FILE *in, char **out, int n, char *header)
{
	char *buffer;
	size_t bufsize = n + 1;
	buffer = (char *)malloc(4 * bufsize * sizeof(char));
	if (buffer == NULL) {
		perror("Unable to allocate buffer");
		exit(1);
	}

	printf("%s", header);
	getline(&buffer, &bufsize, stdin);

	// calculate boundaries and copy the result
	int characters = u8_strlen(buffer);
	int start = characters - n;
	if (start < 0)
		start = 0;
	int end = start + n;
	if (end >= characters)
		end = characters - start;
	int u8_start = u8_offset(buffer, start);
	int u8_end = u8_offset(buffer, end);
	// strncpy(out, buffer + u8_start, u8_end);
	// out[u8_end - 1] = '\0';
	char * res;
	res = (char *)malloc(4 * (u8_end - u8_start) * sizeof(char));
	strncpy(res, buffer + u8_start, u8_end);
	res[u8_end - 1] = '\0';
	*out = res;

	free(buffer);
	return characters;
}
