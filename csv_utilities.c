#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <assert.h>

#include "csv_utilities.h"

int check_header(struct csv_line *out);

struct csv_line default_header = {
	.date = L"Päivämäärä [dd-mm-yyyy]",
	.type = L"Tyyppi [Juoksu('j') | Sali ylävartalo('y') | Sali jalat('a') | Hiihto('h') | Kävely('k') | Crossfit('c') | Uinti('u')]",
	/*.type = L"Tyyppi",*/
	.time = L"Kesto [mmm:ss]",
	.heart_rate = L"Syke avg",
	.heart_rate_max = L"Syke max",
	.distance = L"Matka [km]",
	.evaluation = L"Arvio omasta jaksamisesta [1-5]",
	.description = L"Kuvaus",
};

int parse_csv_line(wchar_t *line, struct csv_line *out)
{
	wchar_t current;
	int index = 0;
	int word_index = 0;
	wchar_t word[LINE_LENGTH];
	memset(word, 0, sizeof word);
	wchar_t **header_field = (wchar_t **)out;
	int num_columns = 0;
	do {
		current = line[index];
		word[word_index] = current;
		if (current == ',' || current == L'\n') {
			wchar_t *field =
				malloc(sizeof(wchar_t) * (word_index + 2));
			// add 2 to prevent random malloc error
			if (!field)
				return -1;
			wcscpy(field, word);
			field[word_index] = L'\0';
			*header_field = field;
			header_field++;

			word_index = 0;
			index++;
			memset(word, 0, sizeof word);
			num_columns++;
		} else {
			index++;
			word_index++;
		}
	} while (current != L'\n');

	assert(num_columns == NUM_HEADERS);
	return 0;
}

int free_csv_line(struct csv_line *line)
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

int check_header(struct csv_line *header)
{
	assert(!wcscmp(header->date, default_header.date));
	assert(!wcscmp(header->type, default_header.type));
	assert(!wcscmp(header->time, default_header.time));
	assert(!wcscmp(header->heart_rate, default_header.heart_rate));
	assert(!wcscmp(header->heart_rate_max, default_header.heart_rate_max));
	assert(!wcscmp(header->distance, default_header.distance));
	assert(!wcscmp(header->evaluation, default_header.evaluation));
	assert(!wcscmp(header->description, default_header.description));
	return 0;
}
