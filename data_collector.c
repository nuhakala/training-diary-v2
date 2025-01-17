#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include <time.h>

#include "csv_utilities.h"
#include "input_utilities.h"
#include "training_utilities.h"
#include "print_utilities.h"

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

	input->date = (char *)malloc(12 * sizeof(char));
	sprintf(input->date, "%02i-%02i-%d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
	input->type = read_char_input(default_header_u8.type, training_types);
	input->time = read_time_input(default_header_u8.time);
	input->distance = read_double_input(default_header_u8.distance, 0.0, 300.0);
	input->heart_rate = read_int_input(default_header_u8.heart_rate, 30, 230);
	input->heart_rate_max = read_int_input(default_header_u8.heart_rate_max, 30, 230);
	input->evaluation = read_int_input(default_header_u8.evaluation, 1, 5);
	input->description = read_string_input(default_header_u8.description, 300);
	input->date = read_date_input(input->date);

	d_printf("Input asked and parsed\n");

	char *file_name = get_save_file_name(argc, argv);
	write_csv_line(input, file_name);
	printf("Tiedot tallennettu.\n");

	d_printf("CSV line written to file %s\n", file_name);

	d_printf("Cleaning up");
	free(file_name);
	free_csv_line(input);
	return 0;
}
