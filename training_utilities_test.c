#include <string.h>
#include <stdlib.h>

#include "training_utilities.h"
#include "csv_utilities.h"
#include "print_utilities.h"

int test_parse_seconds_from_string()
{
	char *input = malloc(sizeof(char) * 20);
	strcpy(input, "12.32");
	int output;
	parse_seconds_from_string(input, &output);
	if (output != 752) {
		print_red(
			"Parse seconds from string failed with input %s and output %d",
			input, output);
		return -1;
	}
	strcpy(input, "12");
	parse_seconds_from_string(input, &output);
	if (output != 720) {
		print_red(
			"Parse seconds from string failed with input %s and output %d",
			input, output);
		return -1;
	}
	strcpy(input, "-12.32");
	if (parse_seconds_from_string(input, &output) >= 0) {
		print_red(
			"Parse seconds from string failed with input %s, did not return error",
			input);
		return -1;
	}
	strcpy(input, "-12.322");
	if (parse_seconds_from_string(input, &output) >= 0) {
		print_red(
			"Parse seconds from string failed with input %s, did not return error",
			input);
		return -1;
	}
	print_green("Parse seconds from string passed");
	free(input);
	return 0;
}

int test_parse_meters_from_string()
{
	char *input = malloc(sizeof(char) * 20);
	strcpy(input, "12.322\0");
	int output;
	parse_meters_from_string(input, &output);
	if (output != 12322) {
		print_red(
			"Parse meters from string failed with input %s and output %d",
			input, output);
		return -1;
	}
	strcpy(input, "5.5\0");
	parse_meters_from_string(input, &output);
	if (output != 5500) {
		print_red(
			"Parse meters from string failed with input %s and output %d",
			input, output);
		return -1;
	}
	strcpy(input, "5.55\0");
	parse_meters_from_string(input, &output);
	if (output != 5550) {
		print_red(
			"Parse meters from string failed with input %s and output %d",
			input, output);
		return -1;
	}
	strcpy(input, "5.555555\0");
	parse_meters_from_string(input, &output);
	if (output != 5555) {
		print_red(
			"Parse meters from string failed with input %s and output %d",
			input, output);
		return -1;
	}
	strcpy(input, "\0");
	parse_meters_from_string(input, &output);
	if (output != 0) {
		print_red(
			"Parse meters from string failed with input %s and output %d",
			input, output);
		return -1;
	}
	strcpy(input, "-5.5\0");
	if (parse_meters_from_string(input, &output) >= 0) {
		print_red(
			"Parse meters from string failed with input %s, did not return error",
			input);
		return -1;
	}
	free(input);
	print_green("Parse meters from string passed");
	return 0;
}

int test_time_to_string()
{
	int input = 330;
	char *output = malloc(sizeof(char) * 12);
	time_to_string(input, output, 10);
	if (strcmp(output, "5m 30s")) {
		print_red("Time to string failed with input %d and output %s",
			  input, output);
		return -1;
	}
	input = 3700;
	time_to_string(input, output, 12);
	if (strcmp(output, "1h 1m")) {
		print_red("Time to string failed with input %d and output %s",
			  input, output);
		return -1;
	}
	input = -1;
	if (time_to_string(input, output, 12) >= 0) {
		print_red(
			"Time to string failed with input %d, did not return error",
			input);
		return -1;
	}
	free(output);
	print_green("Time to string passed");
	return 0;
}

int test_distance_to_string()
{
	int input = 12345;
	char *output = malloc(sizeof(char) * 12);
	distance_to_string(&input, output, 10);
	if (strcmp(output, "12.345 km")) {
		print_red(
			"Distance to string failed with input %d and output %s",
			input, output);
		return -1;
	}
	free(output);
	print_green("Distance to string passed");
	return 0;
}

int test_aggregate_data_points()
{
	struct training_data data = { RUN, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	struct csv_line_u8 input = {
		"13-01-2024",
		"j",
		"48.00",
		"",
		"",
		"",
		"4",
		"cleansquat+askelkyykky+loppuun vähän penaa ja dippejä"
	};
	aggregate_data_points(&data, &input);
	struct csv_line_u8 input2 = {
		"14-01-2024", "j",
		"85.10",      "109",
		"136",	      "5.222",
		"5",	      "olkapäänjumppaus+kädet+keskivartalo"
	};
	aggregate_data_points(&data, &input2);
	struct csv_line_u8 input3 = {
		"16-01-2024", "j",
		"35.03",      "142",
		"164",	      "4.93",
		"4",	      "juoksu+kävelylenkki:keuhkojenkuntoutusta"
	};
	aggregate_data_points(&data, &input3);
	if (data.time != 10093 || data.amount_time != 3) {
		print_red("Aggregate data points failed to count time");
		return -1;
	}
	if (data.amount_heart_rate != 2 || data.heart_rate != 251) {
		print_red("Aggregate data points failed to count heart rate");
		return -1;
	}
	if (data.heart_rate_max != 164) {
		print_red(
			"Aggregate data points failed to find max heart rate");
		return -1;
	}
	if (data.evaluation != 13 || data.amount_evaluation != 3) {
		print_red("Aggregate data points failed to count evaluation");
		return -1;
	}
	if (data.distance != 10152 || data.amount_distance != 2) {
		print_red("Aggregate data points failed to count distance");
		return -1;
	}
	print_green("Aggregrate data points passed");
	return 0;
}

int main(int argc, char *argv[])
{
	int failed = 0;
	int total = 5;
	failed = failed + test_parse_seconds_from_string();
	failed = failed + test_parse_meters_from_string();
	failed = failed + test_time_to_string();
	failed = failed + test_distance_to_string();
	failed = failed + test_aggregate_data_points();
	int passed = total + failed;
	if (passed < total && passed > 0) {
		print_yellow(
			"Training utility tests completed, %d/%d tests passed",
			passed, total);
	}
	if (passed == total) {
		print_green("All training utility tests passed.");
	}
	return 0;
}
