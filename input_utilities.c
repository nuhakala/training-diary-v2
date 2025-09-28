#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wchar.h>

#include "input_utilities.h"
#include "print_utilities.h"
#include "csv_utilities.h"
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

// Reads first *n* characters from input stream *in* with prefix of *header*.
// Store output to *out*.
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
int create_tmp_file(struct csv_line_u8 *input, char * tmp_name)
{
	d_printf("Creating tmp file next.\n");
	int tmp = mkstemp(tmp_name);
	if (tmp == -1) {
		perror("Unable to create temp file");
		return tmp;
	}
	d_printf("Temporary file is created with fd: %d and name: %s\n", tmp, tmp_name);

	// Instructions
	dprintf(tmp, "# Jokainen avain-arvo pari omalle riville.\n");
	dprintf(tmp, "# Älä laita väärää arvoa, koska niitä ei tarkisteta.\n");
	dprintf(tmp, "# Älä muokkaa kaksoispisteen vasenta puolta.\n");
	dprintf(tmp, "# Älä käytä pilkkuja, koska ne sotkee CSV tiedoston.\n\n");

	// Create var so the print statements are clearer
	struct csv_line_u8 dh = default_header_u8;
	dprintf(tmp, "%s > %s\n", dh.date, input->date);
	dprintf(tmp, "%s > %s\n", dh.type, input->type);
	dprintf(tmp, "%s > %s\n", dh.time, input->time);
	dprintf(tmp, "%s > %s\n", dh.heart_rate, input->heart_rate);
	dprintf(tmp, "%s > %s\n", dh.heart_rate_max, input->heart_rate_max);
	dprintf(tmp, "%s > %s\n", dh.distance, input->distance);
	dprintf(tmp, "%s > %s\n", dh.evaluation, input->evaluation);
	dprintf(tmp, "%s > %s\n", dh.description, input->description);

	d_printf("Data written to tmpfile\n");
	return tmp;
}

// This function will assign the value to correct field in given csv_line_u8
// struct.
//
// field = the field where to assign
// value = value which to be assigned
int match_header(char * field, char * value, struct csv_line_u8 *out)
{
	d_printf("Matching header field %s and value %s\n", field, value);
	char **def_header_field = (char **)&default_header_u8;
	char **out_header_field = (char **)out;
	for (int i = 0; i < NUM_HEADERS; i++) {
		if (compare_strings_u8(field, *def_header_field) == 1) {
			*out_header_field = value;
			d_printf("Header matched and value saved\n");
			return 0;
		}
		def_header_field++;
		out_header_field++;
	}
	d_printf("Header not found and value not saved\n");
	return -1;
}

// Parses *line* which is expected to be in tmpfile format.
// Saves the value to the correct field of *out*.
// Returns the number of saved entries, which can be either 1 or 0.
int parse_tmp_line(char *line, struct csv_line_u8 *out)
{
	d_printf("Parsing tmpfile line\n");

	u_int32_t current;
	int word_index = 0;
	int index = 0;
	char word[4 * LINE_LENGTH];
	memset(word, 0, sizeof word);
	char *field = malloc(4 * sizeof(char) * LINE_LENGTH);
	if (!field)
		return -1;
	char *value = malloc(4 * sizeof(char) * LINE_LENGTH);
	if (!value)
		return -1;

	// Iterate over the line character by character.
	do {
		current = u8_nextchar(line, &index);
		// Skip this line
		if (current == '#') {
			d_printf("Encountered comment line, skipping\n");
			while (u8_nextchar(line, &index) != '\n');
			free(field);
			free(value);
			return 0;
		}

		word[word_index] = current;
		// If current char is >, then we have read the key. Copy it to
		// field and reset word. Do the same for value.
		if (current == '>') {
			// add 2 to prevent random malloc error
			strcpy(field, word);
			field[word_index - 1] = '\0';

			word_index = 0;
			memset(word, 0, sizeof word);
		} else if (current == '\n' || current == '\0') {
			strcpy(value, word);
			value[word_index] = '\0';

			word_index = 0;
			memset(word, 0, sizeof word);
		} else {
			word_index++;
		}
	} while (current != '\n');

	d_printf("Line parsed\n");
	// value was not stored
	if (match_header(field, value, out) == -1) {
		free(value);
		free(field);
		return 0;
	}
	// Field not saved anywhere
	free(field);
	return 1;
}

int parse_tmp_file(char * file_fd, struct csv_line_u8 *output)
{
	d_printf("Parsing tmp file\n");
	FILE *fp = fopen(file_fd, "r");

	size_t bufsize = 4 * LINE_LENGTH;
	char *buffer = (char *)malloc(bufsize * sizeof(char));
	int res = 0;
	int num_headers = 0;
	do {
		res = getline(&buffer, &bufsize, fp);
		if (res != -1) {
			num_headers += parse_tmp_line(buffer, output);
		}
	} while (res >= 0); 

	d_printf("tmpfile parsed, num_headers is %d\n", num_headers);
	free(buffer);
	return 0;
}
