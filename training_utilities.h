#ifndef TRAINING_UTILITIES_H

#include <wchar.h>
#include "csv_utilities.h"

#define RUN L"j"
#define GYM_ARMS L"y"
#define GYM_LEGS L"a"
#define WALK L"k"
#define CROSSFIT L"c"
#define SWIM L"u"

extern const char training_types[];

struct training_data {
	wchar_t *type;
	int amount_total;
	int time;
    int amount_time;
	int heart_rate;
	int heart_rate_max;
    int amount_heart_rate;
	int distance;
    int amount_distance;
	int evaluation;
    int amount_evaluation;
};


// For testing purposes, otherwise could be left out
int parse_seconds_from_string(wchar_t *in, int *time);
int parse_meters_from_string(wchar_t *in, int *value);
int time_to_string(int in, char *out, int max);
int distance_to_string(int *in, char *out, int max);

int aggregate_data_points(struct training_data *data, struct csv_line *line);
int print_training_data(struct training_data *data, int include_distance);

#endif // !TRAINING_UTILITIES_H
