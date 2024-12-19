#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include <time.h>

#include "csv_utilities.h"
#include "input_utilities.h"
#include "training_utilities.h"
#include "debug_prints.h"

/*
 * TODO:
 * - tmpfile luonti ja syötteen muokkaaminen editorissa
 * - kuukausittaiset tilastot
 */

int main(int argc, char *argv[])
{
	setlocale(LC_ALL, "");

	struct csv_line_u8 *input = malloc(sizeof(struct csv_line_u8));
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);

	input->date = (char *)malloc(11 * sizeof(char));
	sprintf(input->date, "%d-%d-%d", tm.tm_mday, tm.tm_mon,
		tm.tm_year + 1900);
	input->type = read_char_input(default_header_u8.type, training_types);
	input->time = read_time_input(default_header_u8.time);
	input->heart_rate =
		read_int_input(default_header_u8.heart_rate, 30, 230);
	input->heart_rate_max =
		read_int_input(default_header_u8.heart_rate_max, 30, 230);
	input->distance =
		read_double_input(default_header_u8.distance, 0.0, 300.0);
	input->evaluation = read_int_input(default_header_u8.evaluation, 1, 5);
	input->description =
		read_string_input(default_header_u8.description, 300);

	// Get the save file location
	// If a destination is provided as argument, use it, otherwise
	// use the default which is ./data/<current year>.csv
	char *file_name;
	if (argc == 2) {
		int size = sizeof(argv[1]);
		file_name = (char *)malloc(size * sizeof(char));
		strcpy(file_name, argv[1]);
	} else {
		file_name = (char *)malloc(23 * sizeof(char));
		sprintf(file_name, "./data/%d.csv", tm.tm_year + 1900);
	}
	d_printf("filename: %s\n", file_name);
	write_csv_line(input, file_name);

	free(file_name);
	free_csv_line((struct csv_line *)input);
	return 0;
}
