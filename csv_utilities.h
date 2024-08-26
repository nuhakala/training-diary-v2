#ifndef CSV_UTILITIES_H
#define CSV_UTILITIES_H

#include <wchar.h>

#define LINE_LENGTH 300

struct csv_line {
	wchar_t *date;
	wchar_t *type;
	wchar_t *time;
	wchar_t *heart_rate;
	wchar_t *heart_rate_max;
	wchar_t *distance;
	wchar_t *evaluation;
	wchar_t *description;
};

#define NUM_HEADERS 8
extern struct csv_line default_header;
int parse_csv_line(wchar_t *line, struct csv_line *out);
int free_csv_line(struct csv_line *header);
int check_header(struct csv_line *header);

#endif
