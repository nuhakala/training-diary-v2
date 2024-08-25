#define LINE_LENGTH 300

struct measurement {
	void *value;
	char *description;
	char *unit;
	int (*read_input)();
};

struct csv_line {
	wchar_t *date;
	int type;
	int time;
	int heart_rate;
	int heart_rate_max;
	int distance;
	int evaluation;
	wchar_t *description;
};

struct csv_header {
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
extern struct csv_header default_header;
int parse_header(wchar_t *line, struct csv_header *out);
int free_header(struct csv_header *header);
