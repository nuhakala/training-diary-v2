#include "debug_prints.h"
#include "training_utilities.h"
#include <stdio.h>
#include <stdlib.h>

#include "csv_utilities.h"
#include "input_utilities.h"

int main(int argc, char *argv[])
{
	wprintf(L"%ls\n", default_header.type);
        wprintf(L">>> ");
	char *type = malloc(sizeof(wchar_t) * 2);
        read_first_n(stdin, type, 2);
        d_wprintf(L"Type read, %s\n", type);

	wprintf(L"%ls\n", default_header.time);
        wprintf(L">>> ");
        fflush(stdout);
        wchar_t *time = malloc(sizeof(wchar_t) * 10);
        read_first_n_wide(stdin, time, 10);
        int seconds;
        parse_seconds_from_string(time, &seconds);
        d_wprintf(L"Time read: %d seconds\n", seconds);

        d_wprintf(L"Cleaning up\n");
        free(type);
        free(time);
	return 0;
}
