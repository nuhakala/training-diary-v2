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
		printf("File not found");
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

	struct training_data run = { RUN, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	struct training_data arms = { GYM_ARMS, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	struct training_data legs = { GYM_LEGS, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	struct training_data walk = { WALK, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	struct training_data crossfit = { CROSSFIT, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	struct training_data swim = { SWIM, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	struct training_data all = { "0", 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

	while (getline(&buff, &l, fp) >= 0) {
		struct csv_line_u8 *line = malloc(sizeof(struct csv_line_u8));
		parse_csv_line_u8(buff, line);

		aggregate_data_points(&all, line);
		if (compare_strings_u8(line->type, RUN)) {
			aggregate_data_points(&run, line);
		} else if (compare_strings_u8(line->type, GYM_ARMS)) {
			aggregate_data_points(&arms, line);
		} else if (compare_strings_u8(line->type, GYM_LEGS)) {
			aggregate_data_points(&legs, line);
		} else if (compare_strings_u8(line->type, WALK)) {
			aggregate_data_points(&walk, line);
		} else if (compare_strings_u8(line->type, CROSSFIT)) {
			aggregate_data_points(&crossfit, line);
		} else if (compare_strings_u8(line->type, SWIM)) {
			aggregate_data_points(&swim, line);
		}
		free_csv_line(line);
	}
	d_printf("File read and data collected\n");

	printf("Koko vuoden tulokset");
	printf("Juoksu:\n");
	print_training_data(&run, 1);
	printf("Sali ylävartalo:\n");
	print_training_data(&arms, 0);
	printf("Sali jalat:\n");
	print_training_data(&legs, 0);
	printf("Kävely:\n");
	print_training_data(&walk, 1);
	printf("Crossfit:\n");
	print_training_data(&crossfit, 0);
	printf("Uinti:\n");
	print_training_data(&swim, 1);
	printf("Kaikki yhdessä:\n");
	print_training_data(&all, 1);

	d_printf("Cleaning up\n");
	fclose(fp);
	free(buff);
	return 0;
}
