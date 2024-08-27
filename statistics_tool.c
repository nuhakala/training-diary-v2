#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
#include <wchar.h>
#include <assert.h>

#include "csv_utilities.h"
#include "training_utilities.h"
#include "debug_prints.h"

int main(int argc, char *argv[])
{
        // Needed for correct reading of file
        setlocale(LC_CTYPE, "fi_FI.UTF-8");

	FILE *fp;
	fp = fopen("./data/2024.csv", "r");

	if (!fp) {
		printf("File not found");
		return -1;
	}

	wchar_t *buff = malloc(sizeof(wchar_t) * (LINE_LENGTH + 1));
	if (!fgetws(buff, LINE_LENGTH, fp)) {
		printf("Error reading header");
		return -1;
	}

	struct csv_line *header = malloc(sizeof(struct csv_line));
	parse_csv_line(buff, header);
        check_header(header);
        free_csv_line(header);
        d_printf("Header parsed and checked\n");

        struct training_data run = {RUN,0,0,0,0,0,0,0,0,0,0};
        struct training_data arms = {GYM_ARMS,0,0,0,0,0,0,0,0,0,0};
        struct training_data legs = {GYM_LEGS,0,0,0,0,0,0,0,0,0,0};
        struct training_data walk = {WALK,0,0,0,0,0,0,0,0,0,0};
        struct training_data crossfit = {CROSSFIT,0,0,0,0,0,0,0,0,0,0};
        struct training_data swim = {SWIM,0,0,0,0,0,0,0,0,0,0};

        while (fgetws(buff, LINE_LENGTH, fp)) {
                struct csv_line *line = malloc(sizeof(struct csv_line));
                parse_csv_line(buff, line);

                if (!wcscmp(line->type, RUN)) {
                        aggregate_data_points(&run, line);
                } else if (!wcscmp(line->type, GYM_ARMS)) {
                        aggregate_data_points(&arms, line);
                } else if (!wcscmp(line->type, GYM_LEGS)) {
                        aggregate_data_points(&legs, line);
                } else if (!wcscmp(line->type, WALK)) {
                        aggregate_data_points(&walk, line);
                } else if (!wcscmp(line->type, CROSSFIT)) {
                        aggregate_data_points(&crossfit, line);
                } else if (!wcscmp(line->type, SWIM)) {
                        aggregate_data_points(&swim, line);
                }
                free_csv_line(line);
        }
        d_printf("File read and data collected\n");

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

        d_printf("Cleaning up\n");

	fclose(fp);
	free(buff);
	return 0;
}
