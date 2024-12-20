#ifndef CSV_UTILITIES_H
#define CSV_UTILITIES_H

#include <wchar.h>

#define LINE_LENGTH 300

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
extern struct csv_line_u8 default_header_u8;
int parse_csv_line_u8(char *line, struct csv_line_u8 *out);
int free_csv_line(struct csv_line_u8 *header);
int check_header_u8(struct csv_line_u8 *header);

char *get_save_file_name(int argc, char *argv[]);
int write_csv_line(struct csv_line_u8 *line, char *line_name);

#endif
