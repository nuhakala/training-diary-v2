#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <assert.h>

#include "csv-utilities.h"

struct csv_header default_header = {
	.date = L"Päivämäärä [dd-mm-yyyy]",
	.type = L"Tyyppi [Juoksu('j') | Sali ylävartalo('y') | Sali jalat('a') | Hiihto('h') | Kävely('k') | Crossfit('c') | Uinti('u')]",
	.time = L"Kesto [mmm:ss]",
	.heart_rate = L"Syke avg",
	.heart_rate_max = L"Syke max",
	.distance = L"Matka [km]",
	.evaluation = L"Arvio omasta jaksamisesta [1-5]",
	.description = L"Kuvaus",
};

int parse_header(wchar_t *line, struct csv_header *out)
{
	wchar_t current = line[0];
	wchar_t word[LINE_LENGTH];
	memset(word, 0, sizeof word);
	int index = 0;
	int word_index = 0;
	wchar_t **header_field = (wchar_t **)out;
	int num_columns = 0;
	while (current != '\n') {
		if (current == ',' || current == L'\n') {
			wchar_t *field =
				malloc(sizeof(wchar_t) * (word_index + 1));
			wcscpy(field, word);
                        field[word_index + 1] = L'\0';
			*header_field = field;
			header_field++;

                        switch (num_columns) {
                                case 0:
                                        // wcscmp returns 0 if equals
                                        assert(!wcscmp(field, default_header.date));
                                        break;
                                case 1:
                                        assert(!wcscmp(field, default_header.type));
                                        break;
                                case 2:
                                        assert(!wcscmp(field, default_header.time));
                                        break;
                                case 3:
                                        assert(!wcscmp(field, default_header.heart_rate));
                                        break;
                                case 4:
                                        assert(!wcscmp(field, default_header.heart_rate_max));
                                        break;
                                case 5:
                                        assert(!wcscmp(field, default_header.distance));
                                        break;
                                case 6:
                                        assert(!wcscmp(field, default_header.evaluation));
                                        break;
                                case 7:
                                        assert(!wcscmp(field, default_header.description));
                                        break;
                        }

			word_index = 0;
			index++; // skip comma
			current = line[index];
			memset(word, 0, sizeof word);
			num_columns++;
		}
		word[word_index] = current;
		index++;
		current = line[index];
		word_index++;
	}
	assert(num_columns == NUM_HEADERS - 1);
	return 0;
}

int parse_csv_line(wchar_t *line, struct csv_line *out)
{

	wchar_t current = line[0];
	wchar_t word[LINE_LENGTH];
	memset(word, 0, sizeof word);
	int index = 1;
	int word_index = 0;
	int num_columns = 0;
	while (current != '\n') {
		if (current == ',') {
			wchar_t *field =
				malloc(sizeof(wchar_t) * (word_index + 1));
			wcscpy(field, word);
			printf("%ls\n", field);

			word_index = 0;
			index++; // skip comma
			current = line[index];
			memset(word, 0, sizeof word);
			num_columns++;
		}

		word[word_index] = current;
		index++;
		current = line[index];
		word_index++;
	}
	assert(num_columns == NUM_HEADERS - 1);
	return 0;
}

int free_header(struct csv_header *header)
{
        free(header->heart_rate);
        free(header->distance);
        free(header->time);
        free(header->heart_rate_max);
        free(header->type);
        free(header->evaluation);
        free(header->date);
        free(header->description);
        free(header);
        return 0;
}
