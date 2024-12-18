#include <stdlib.h>
#include <locale.h>

#include "csv_utilities.h"
#include "input_utilities.h"
#include "training_utilities.h"
#include "debug_prints.h"

int main(int argc, char *argv[])
{
	setlocale(LC_ALL, "");

	struct csv_line_u8 *res = malloc(sizeof(struct csv_line_u8));

	char c_options[] = { 'j', 'y', 'a', 'h', 'k', 'c', 'u' };
	res->type = read_char_input(
		"Tyyppi [Juoksu('j') | Sali ylävartalo('y') | Sali jalat('a') | Hiihto('h') | Kävely('k') | Crossfit('c') | Uinti('u')] >>> ",
		c_options);
	res->time = read_time_input("Kesto [mmm.ss] >>> ");
	res->heart_rate = read_int_input("Syke avg >>> ", 30, 230);
	res->heart_rate_max = read_int_input("Syke max >>> ", 30, 230);
	res->distance = read_double_input("Matka [km] >>> ", 0.0, 300.0);
	res->evaluation = read_int_input("Arvio omasta jaksamisesta [1-5] >>> ", 1, 5);
	res->description = read_string_input("Kuvaus >>> ", 300);

	free_csv_line((struct csv_line *)res);
	return 0;
}
