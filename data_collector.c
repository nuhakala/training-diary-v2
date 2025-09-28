#include <bits/types/idtype_t.h>
#include <bits/types/siginfo_t.h>
#include <stdlib.h>
#include <locale.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>

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
	sprintf(input->date, "%02i-%02i-%d", tm.tm_mday, tm.tm_mon + 1,
		tm.tm_year + 1900);
	input->type = read_char_input(default_header_u8.type, training_types);
	input->time = read_time_input(default_header_u8.time);
	input->distance =
		read_double_input(default_header_u8.distance, 0.0, 300.0);
	input->heart_rate =
		read_int_input(default_header_u8.heart_rate, 30, 230);
	input->heart_rate_max =
		read_int_input(default_header_u8.heart_rate_max, 30, 230);
	input->evaluation = read_int_input(default_header_u8.evaluation, 1, 5);
	input->description =
		read_string_input(default_header_u8.description, 300);
	input->date = read_date_input(input->date);

	d_printf("Input asked and parsed\n");

	// Edit input if needed
	const char edit_options[] = { 'Y', 'y', 'N', 'n' };
	char *edit =
		read_char_input("Haluatko muokata tietoja [y/n]", edit_options);
	int tmp_file_fd;
	// The following mostly copied from
	// https://stackoverflow.com/questions/11221186/how-do-i-find-a-filename-given-a-file-pointer
	// and
	// https://stackoverflow.com/questions/34625123/how-to-correctly-wait-for-execve-to-finish
	if (*edit == 'Y' || *edit == 'y') {
		char tmp_name[] = "training_collector_tmp.XXXXXX";
		tmp_file_fd = create_tmp_file(input, tmp_name);
		if (tmp_file_fd < 0) {
			exit(1);
		}

		d_printf("Forking process next.\n");

		char *arguments[] = { "nvim", tmp_name, NULL };
		pid_t pid = fork();
		if (pid < 0) {
			perror("fork failed");
			exit(EXIT_FAILURE);
		} else if (pid == 0) {
			if (execv("/usr/bin/nvim", arguments)) {
				perror("Failed to open nvim on temp file");
				exit(127);
			};
			// not reached!
		};
		d_printf("Fork created, waiting for child to exit.\n");
		waitid(P_PID, pid, NULL, WEXITED);
		d_printf(
			"Neovim exited, the temp file should now have stuff\n");

		// Free input and allocate it again
		free_csv_line(input);
		input = malloc(sizeof(struct csv_line_u8));
		parse_tmp_file(tmp_name, input);

		// Clean up the tmp file
		unlink(tmp_name);
		close(tmp_file_fd);
	}

	char *file_name = get_save_file_name(argc, argv);
	write_csv_line(input, file_name);
	printf("Tiedot tallennettu.\n");

	d_printf("CSV line written to file %s\n", file_name);

	d_printf("Cleaning up\n");
	free(file_name);
	free_csv_line(input);
	return 0;
}
