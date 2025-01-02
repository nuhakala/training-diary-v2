#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
#include <wchar.h>
#include <assert.h>

#include "csv_utilities.h"
#include "training_utilities.h"
#include "print_utilities.h"
#include "utf8.h"

int main(int argc, char *argv[])
{
	// Needed for correct reading of file
	setlocale(LC_ALL, "");

	FILE *fp;
	char *file_name = get_save_file_name(argc, argv);
	d_printf("Trying to read file %s\n", file_name);

	fp = fopen(file_name, "r");
	if (!fp) {
		printf("File %s not found, aborting program", file_name);
		return -1;
	}

	char *buff = malloc(sizeof(wchar_t) * (LINE_LENGTH + 1));
	size_t l = LINE_LENGTH;
	if (!getline(&buff, &l, fp)) {
		printf("Error reading header");
		return -1;
	}

	struct csv_line_u8 *header = malloc(sizeof(struct csv_line_u8));
	parse_csv_line_u8(buff, header);
	check_header_u8(header);
	free_csv_line(header);
	d_printf("Header parsed and checked\n");

	struct training_data **run = initialize_data_array(RUN, 13);
	struct training_data **arms = initialize_data_array(GYM_ARMS, 13);
	struct training_data **legs = initialize_data_array(GYM_LEGS, 13);
	struct training_data **walk = initialize_data_array(WALK, 13);
	struct training_data **crossfit = initialize_data_array(CROSSFIT, 13);
	struct training_data **swim = initialize_data_array(SWIM, 13);
	struct training_data **indoor = initialize_data_array(INDOOR, 13);
	struct training_data all = empty_data("0")

	d_printf("Entering line-reading loop\n");
	struct csv_line_u8 *line = malloc(sizeof(struct csv_line_u8));
	while (getline(&buff, &l, fp) >= 0)
	{
		if (!line) {
			printf("Malloc failed, aborting\n");
		}
		parse_csv_line_u8(buff, line);

		aggregate_data_points(&all, line);
		if (compare_strings_u8(line->type, RUN)) {
			aggregate_data_points_array(run, line);
		} else if (compare_strings_u8(line->type, GYM_ARMS)) {
			aggregate_data_points_array(arms, line);
		} else if (compare_strings_u8(line->type, GYM_LEGS)) {
			aggregate_data_points_array(legs, line);
		} else if (compare_strings_u8(line->type, WALK)) {
			aggregate_data_points_array(walk, line);
		} else if (compare_strings_u8(line->type, CROSSFIT)) {
			aggregate_data_points_array(crossfit, line);
		} else if (compare_strings_u8(line->type, SWIM)) {
			aggregate_data_points_array(swim, line);
		} else if (compare_strings_u8(line->type, INDOOR)) {
			aggregate_data_points_array(indoor, line);
		}
	}
	free_csv_line(line);
	d_printf("File read and data collected\n");

	print_green("Tiedoston %s tilastot:", file_name);
	print_yellow("Juoksu:");
	print_training_data_array(run, 13, 1);
	print_yellow("Sali ylävartalo:");
	print_training_data_array(arms, 13, 0);
	print_yellow("Sali jalat:");
	print_training_data_array(legs, 13, 0);
	print_yellow("Kävely:");
	print_training_data_array(walk, 13, 1);
	print_yellow("Crossfit:");
	print_training_data_array(crossfit, 13, 0);
	print_yellow("Uinti:");
	print_training_data_array(swim, 13, 1);
	print_yellow("Sisäliikunta:");
	print_training_data_array(indoor, 13, 0);
	print_green("Kaikki yhdessä:");
	print_training_data(&all, 1);

	d_printf("Cleaning up\n");
	fclose(fp);
	free(buff);
	free_data_array(run, 13);
	free_data_array(arms, 13);
	free_data_array(legs, 13);
	free_data_array(walk, 13);
	free_data_array(crossfit, 13);
	free_data_array(swim, 13);
	free_data_array(indoor, 13);
	
	printf("Paina mitä vain poistuaksesi");
	char x;
	scanf("%c", &x);
	return 0;
}
