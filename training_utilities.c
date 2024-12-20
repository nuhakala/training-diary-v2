#include "print_utilities.h"
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "training_utilities.h"

const char training_types[] = { 'j', 'y', 'a', 'h', 'k', 'c', 'u' };

int parse_int_value(char *in, int *value)
{
	return sscanf(in, "%d", value);
}

/**
 * @param in string in format mm.ss
 */
int parse_seconds_from_string(char *in, int *time)
{
	int minutes = 0, seconds = 0;
	int res = sscanf(in, "%d.%d", &minutes, &seconds);
	if (seconds > 60 || minutes < 0)
		return -1;
	*time = minutes * 60 + seconds;
	return res;
}

int parse_seconds_from_string_(char *in, int *time)
{
	int minutes = 0, seconds = 0;
	int res = scanf(in, "%d.%d", &minutes, &seconds);
	if (seconds > 60 || minutes < 0)
		return -1;
	*time = minutes * 60 + seconds;
	return res;
}

int parse_meters_from_string(char *in, int *value)
{
	double km = 0;
	int res = sscanf(in, "%lf", &km);
	if (km < 0)
		return -1;
	*value = km * 1000;
	return res;
}

/**
 * @param in time in seconds
 * @param out string to write the result
 * @param max max characters to write
 * @return number of characters written
 */
int time_to_string(int in, char *out, int max)
{
	if (in < 0)
		return -1;
	int minutes = in / 60;
	int hours = minutes / 60;
	int seconds = in % 60;
	if (hours > 0) {
		return snprintf(out, max, "%dh %dm", hours, minutes % 60);
	} else {
		return snprintf(out, max, "%dm %ds", minutes, seconds);
	}
}

/**
 * @param in distance in meters
 * @param out string to write the number to in kilometers
 * @param max max characters to write
 * @return number of characters written
 */
int distance_to_string(int *in, char *out, int max)
{
	int km = *in / 1000;
	int meters = *in % 1000;
	return snprintf(out, max, "%d.%d km", km, meters);
}

int aggregate_data_points(struct training_data *data, struct csv_line_u8 *line)
{
	data->amount_total++;
	int time = 0;
	if (0 < parse_seconds_from_string(line->time, &time)) {
		data->time = data->time + time;
		data->amount_time++;
	}

	int heart_rate = 0;
	if (0 < parse_int_value(line->heart_rate, &heart_rate)) {
		data->heart_rate = data->heart_rate + heart_rate;
		data->amount_heart_rate++;
	}

	int hrm = 0;
	if (0 < parse_int_value(line->heart_rate_max, &hrm)) {
		data->heart_rate_max =
			data->heart_rate_max > hrm ? data->heart_rate_max : hrm;
	}

	int dist = 0;
	if (0 < parse_meters_from_string(line->distance, &dist)) {
		data->distance = data->distance + dist;
		data->amount_distance++;
	}

	int evaluation = 0;
	if (0 < parse_int_value(line->evaluation, &evaluation)) {
		data->evaluation = data->evaluation + evaluation;
		data->amount_evaluation++;
	}

	return 0;
}

int aggregate_data_points_array(struct training_data **in, struct csv_line_u8 *line)
{
	int day, month, year;
	sscanf(line->date, "%d-%d-%d", &day, &month, &year);
	aggregate_data_points(in[month], line); // aggregate monthly
	aggregate_data_points(in[0], line); // aggregate all time
	return 0;
}

int print_training_data_array(struct training_data **in, int n, int include_distance)
{
	for (int i = 1; i < n; i++) {
		print_yellow("Kuukausi %d:", i);
		print_training_data(in[i], include_distance);
	}
	print_yellow("Yhteensä:");
	print_training_data(in[0], include_distance);
	return 0;
}

int print_training_data(struct training_data *data, int include_distance)
{
	printf("Yhteensä %d kertaa\n", data->amount_total);

	char time_total[11] = { 0 }, time_avg[11] = { 0 };
	time_to_string(data->time, time_total, 10);
	int time = data->amount_time > 0 ? data->time / data->amount_time : 0;
	time_to_string(time, time_avg, 10);
	printf("Aikaa tuhlattu yhteensä %s, keskimäärin %s per kerta\n",
	       time_total, time_avg);

	int hr_avg = data->amount_heart_rate > 0 ?
			     data->heart_rate / data->amount_heart_rate :
			     0;
	printf("Keskisyke %d, korkein kaikista %d\n", hr_avg,
	       data->heart_rate_max);

	if (include_distance) {
		char dist_total[12] = { 0 }, dist_avg[11] = { 0 };
		distance_to_string(&data->distance, dist_total, 11);
		int dist_avg_meter =
			data->amount_distance > 0 ?
				data->distance / data->amount_distance :
				0;
		distance_to_string(&dist_avg_meter, dist_avg, 10);
		printf("Matkaa yhteensä %s, keskimäärin %s\n", dist_total,
		       dist_avg);
	}

	printf("\n");

	return 0;
}

struct training_data **initialize_data_array(char *type, int n)
{
	struct training_data **a = malloc(n * sizeof(struct training_data *));
	// struct training_data a[n];
	for (int i = 0; i < n; i++) {
		struct training_data *d = malloc(sizeof(struct training_data));
		struct training_data t = empty_data(type);
		memcpy(d, &t, sizeof(struct training_data));
		a[i] = d;
	}
	return a;
}

void free_data_array(struct training_data **a, int n)
{
	for (int i = 0; i < n; i++) {
		free(a[i]);
	}
	free(a);
}
