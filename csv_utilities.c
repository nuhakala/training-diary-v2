#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <wchar.h>
#include <assert.h>
#include <unistd.h>
#include <time.h>

#include "csv_utilities.h"
#include "utf8.h"

// Header constants, names are little misleading :(

// The header constants that are asked from the user
struct csv_line_u8 default_header_u8 = {
	.date = "Päivämäärä [dd-mm-yyyy]",
	.type = "Tyyppi [Juoksu('j') | Sali ylävartalo('y') | Sali jalat('a') | Hiihto('h') | Kävely('k') | Crossfit('c') | Uinti('u') | Sisäliikunta('s')]",
	.time = "Kesto [mmm:ss]",
	.heart_rate = "Syke avg",
	.heart_rate_max = "Syke max",
	.distance = "Matka [km]",
	.evaluation = "Arvio omasta jaksamisesta [1-5]",
	.description = "Kuvaus",
};

// The header constants that are written to file, and should also be present in file
struct csv_line_u8 correct_header = {
	.date = "Päivämäärä [dd-mm-yyyy]",
	.type = "Tyyppi [kirjain]",
	.time = "Kesto [mmm:ss]",
	.heart_rate = "Syke avg",
	.heart_rate_max = "Syke max",
	.distance = "Matka [km]",
	.evaluation = "Arvio omasta jaksamisesta [1-5]",
	.description = "Kuvaus",
};

int parse_csv_line_u8(char *line, struct csv_line_u8 *out)
{
	u_int32_t current;
	int index = 0;
	int word_index = 0;
	char word[4 * LINE_LENGTH];
	memset(word, 0, sizeof word);
	char **header_field = (char **)out;
	int num_columns = 0;
	do {
		current = u8_nextchar(line, &index);
		word[word_index] = current;
		if (current == ',' || current == '\n') {
			// add 2 to prevent random malloc error
			char *field =
				malloc(4 * sizeof(char) * (word_index + 2));
			if (!field)
				return -1;
			strcpy(field, word);
			field[word_index] = '\0';
			*header_field = field;
			header_field++;

			word_index = 0;
			memset(word, 0, sizeof word);
			num_columns++;
		} else {
			word_index++;
		}
	} while (current != '\n');

	assert(num_columns == NUM_HEADERS);
	return 0;
}

int free_csv_line(struct csv_line_u8 *line)
{
	free(line->heart_rate);
	free(line->distance);
	free(line->time);
	free(line->heart_rate_max);
	free(line->type);
	free(line->evaluation);
	free(line->date);
	free(line->description);
	free(line);
	return 0;
}

/*
 * We have to do manual checking character by character, because for some
 * reason strcmp function fails even though the utf8 encoded strings contain
 * exactly the same characters.
 */
int check_header_u8(struct csv_line_u8 *header)
{
	printf("header: %s, correct: %s\n", header->type, correct_header.type);
	assert(compare_strings_u8(header->date, correct_header.date));
	assert(compare_strings_u8(header->type, correct_header.type));
	assert(compare_strings_u8(header->time, correct_header.time));
	assert(compare_strings_u8(header->heart_rate, correct_header.heart_rate));
	assert(compare_strings_u8(header->heart_rate_max, correct_header.heart_rate_max));
	assert(compare_strings_u8(header->distance, correct_header.distance));
	assert(compare_strings_u8(header->evaluation, correct_header.evaluation));
	assert(compare_strings_u8(header->description, correct_header.description));
	return 0;
}

int write_csv_line(struct csv_line_u8 *line, char *file_name)
{
	FILE *fptr = NULL;

	// Initialize header if file does not exist
	if (access(file_name, F_OK) != 0) {
		fptr = fopen(file_name, "a");
		fprintf(fptr, "%s,", correct_header.date);
		fprintf(fptr, "%s,", correct_header.type);
		fprintf(fptr, "%s,", correct_header.time);
		fprintf(fptr, "%s,", correct_header.heart_rate);
		fprintf(fptr, "%s,", correct_header.heart_rate_max);
		fprintf(fptr, "%s,", correct_header.distance);
		fprintf(fptr, "%s,", correct_header.evaluation);
		fprintf(fptr, "%s\n", correct_header.description);
	}

	// write the actual content
	if (!fptr)
		fptr = fopen(file_name, "a");
	fprintf(fptr, "%s,", line->date);
	fprintf(fptr, "%s,", line->type);
	fprintf(fptr, "%s,", line->time);
	fprintf(fptr, "%s,", line->heart_rate);
	fprintf(fptr, "%s,", line->heart_rate_max);
	fprintf(fptr, "%s,", line->distance);
	fprintf(fptr, "%s,", line->evaluation);
	fprintf(fptr, "%s", line->description);
	fclose(fptr);
	return 0;
}

char *get_save_file_name(int argc, char *argv[])
{
	// If a destination is provided as argument to program, use it, otherwise
	// use the default which is ./data/<current year>.csv
	char *file_name;
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	if (argc == 2) {
		int size = sizeof(argv[1]);
		file_name = (char *)malloc(size * sizeof(char));
		strcpy(file_name, argv[1]);
	} else {
		file_name = (char *)malloc(23 * sizeof(char));
		sprintf(file_name, "./data/%d.csv", tm.tm_year + 1900);
	}
	return file_name;
}
