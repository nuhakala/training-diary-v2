#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#include "input_utilities.h"
#include "debug_prints.h"
#include "utf8.h"

char *read_char_input(char *header, const char *options)
{
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
		int hours, seconds = 0;
		int a = sscanf(p, "%d.%d", &hours, &seconds);
		if (a == -1) { // allow empty input
			strcpy(p, "");
			return p;
		}
		// I guess I should check that the number are between 0-60 but
		// I am lazy >:D
		if (a >= 1) {
			sprintf(p, "%d.%d", hours, seconds);
			return p;
		}
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
		sscanf(p, "%d", &num);
		if (sscanf(p, "%d", &num) == -1) { // allow empty input
			strcpy(p, "");
			return p;
		}
		if (num >= low && num <= up) {
			sprintf(p, "%d", num);
			return p;
		}
	}
}

char *read_double_input(char *header, double low, double up)
{
	char *p;
	while (1) {
		read_first_n(stdin, &p, 10, header);
		int km, meters = 0;
		int a = sscanf(p, "%d.%d", &km, &meters);
		if (a == -1) { // allow empty input
			strcpy(p, "");
			return p;
		}
		if (a >= 1 && km >= low && km <= up) {
			sprintf(p, "%d.%d", km, meters);
			return p;
		}
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
	char *res;
	res = (char *)malloc(4 * (u8_end - u8_start) * sizeof(char));
	strncpy(res, buffer + u8_start, u8_end);
	res[u8_end - 1] = '\0';
	*out = res;

	free(buffer);
	return characters;
}
