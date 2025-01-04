#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#include "input_utilities.h"
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

char *read_date_input(char *default_date)
{
	char *p;
	while (1) {
		read_first_n(stdin, &p, 11, default_date);
		int d = 0, m = 0, y = 0;
		int a = sscanf(p, "%d.%d.%d", &d, &m, &y);
		if (a == -1) { // allow empty input
			strcpy(p, default_date);
			return p;
		}
		if (a >= 1 && d > 0 && d < 32 && m > 0 && m < 13 && y > 2000 && y < 2100) {
			sprintf(p, "%02d-%02d-%d", d, m, y);
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

	printf("%s >>> ", header);
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

	printf("%s >>> ", header);
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

// Create tmp file for editing input, not used atm
/* int create_tmp_file(struct csv_line_u8 *input)
{
	FILE *tmp = tmpfile();
	if (tmp == NULL) {
		printf("Unable to create temp file");
		return 0;
	}
	d_printf("Temporary file is created\n");

	// Instructions
	fprintf(tmp, "# Jokainen avain-arvo pari omalle riville.\n");
	fprintf(tmp, "# Älä laita väärää arvoa, koska niitä ei tarkisteta.\n");
	fprintf(tmp, "# Älä muokkaa kaksoispisteen vasenta puolta.\n");
	fprintf(tmp, "# Älä käytä pilkkuja, koska ne sotkee CSV tiedoston.\n\n");

	struct csv_line_u8 dh = default_header_u8;
	fprintf(tmp, "%s >>> %s\n", dh.date, input->date);
	fprintf(tmp, "%s >>> %s\n", dh.type, input->type);
	fprintf(tmp, "%s >>> %s\n", dh.time, input->time);
	fprintf(tmp, "%s >>> %s\n", dh.heart_rate, input->heart_rate);
	fprintf(tmp, "%s >>> %s\n", dh.heart_rate_max, input->heart_rate_max);
	fprintf(tmp, "%s >>> %s\n", dh.distance, input->distance);
	fprintf(tmp, "%s >>> %s\n", dh.evaluation, input->evaluation);
	fprintf(tmp, "%s >>> %s\n", dh.description, input->description);

	return 1;
} */
