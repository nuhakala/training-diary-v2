#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
#include <wchar.h>
#include <assert.h>

#include "csv_utilities.h"

int main(int argc, char *argv[])
{
        // Needed for correct reading of file
        setlocale(LC_CTYPE, "fi_FI.UTF-8");

	FILE *fp;
	fp = fopen("./treenit.csv", "r");

	if (!fp) {
		printf("File not found");
		return 1;
	}

	wchar_t *buff = malloc(sizeof(wchar_t) * (LINE_LENGTH + 1));
	if (!fgetws(buff, LINE_LENGTH, fp)) {
		printf("Error reading header");
		return 1;
	}

	struct csv_line *header = malloc(sizeof(struct csv_line));
	parse_csv_line(buff, header);
        check_header(header);

	printf("jottain: %ls, %ls, %ls\n", header->date, header->time,
	       header->type);

	fclose(fp);
	free(buff);
        free_header(header);
	return 0;
}
