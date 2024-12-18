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

struct csv_line_u8 {
	char *date;
	char *type;
	char *time;
	char *heart_rate;
	char *heart_rate_max;
	char *distance;
	char *evaluation;
	char *description;
};

#define NUM_HEADERS 8
extern struct csv_line default_header;
int parse_csv_line(wchar_t *line, struct csv_line *out);
int free_csv_line(struct csv_line *header);
int check_header(struct csv_line *header);

// int get_csv_file_name(char *name);
int write_csv_line(struct csv_line_u8 *line, char *line_name);

#endif
