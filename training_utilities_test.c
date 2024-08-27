#include <string.h>
#include <wchar.h>
#include <stdlib.h>
#include <stdio.h>

#include "training_utilities.h"
#include "csv_utilities.h"
#include "test_utilities.h"

int test_parse_seconds_from_string()
{
	wchar_t *input = malloc(sizeof(wchar_t) * 20);
	wcscpy(input, L"12.32");
	int output;
	parse_seconds_from_string(input, &output);
	if (output != 752) {
		test_failed(
			"Parse seconds from string failed with input %ls and output %d",
			input, output);
		return -1;
	}
	wcscpy(input, L"-12.32");
	if (parse_seconds_from_string(input, &output) >= 0) {
		test_failed(
			"Parse seconds from string failed with input %ls, did not return error",
			input);
		return -1;
	}
	wcscpy(input, L"-12.322");
	if (parse_seconds_from_string(input, &output) >= 0) {
		test_failed(
			"Parse seconds from string failed with input %ls, did not return error",
			input);
		return -1;
	}
	test_passed("Parse seconds from string passed");
	return 0;
}

int test_parse_meters_from_string()
{
	wchar_t *input = malloc(sizeof(wchar_t) * 20);
	wcscpy(input, L"12.322\0");
	int output;
	parse_meters_from_string(input, &output);
	if (output != 12322) {
		test_failed(
			"Parse meters from string failed with input %ls and output %d",
			input, output);
		return -1;
	}
	wcscpy(input, L"5.5\0");
	parse_meters_from_string(input, &output);
	if (output != 5500) {
		test_failed(
			"Parse meters from string failed with input %ls and output %d",
			input, output);
		return -1;
	}
	wcscpy(input, L"5.55\0");
	parse_meters_from_string(input, &output);
	if (output != 5550) {
		test_failed(
			"Parse meters from string failed with input %ls and output %d",
			input, output);
		return -1;
	}
	wcscpy(input, L"5.555555\0");
	parse_meters_from_string(input, &output);
	if (output != 5555) {
		test_failed(
			"Parse meters from string failed with input %ls and output %d",
			input, output);
		return -1;
	}
	wcscpy(input, L"\0");
	parse_meters_from_string(input, &output);
	if (output != 0) {
		test_failed(
			"Parse meters from string failed with input %ls and output %d",
			input, output);
		return -1;
	}
	wcscpy(input, L"-5.5\0");
	if (parse_meters_from_string(input, &output) >= 0) {
		test_failed(
			"Parse meters from string failed with input %ls, did not return error",
			input);
		return -1;
	}
	free(input);
	test_passed("Parse meters from string passed");
	return 0;
}

int test_time_to_string()
{
	int input = 330;
	char *output = malloc(sizeof(char) * 12);
	time_to_string(input, output, 10);
	if (strcmp(output, "5m 30s")) {
		test_failed("Time to string failed with input %d and output %s",
			    input, output);
		return -1;
	}
	input = 3700;
	time_to_string(input, output, 12);
	if (strcmp(output, "1h 1m")) {
		test_failed("Time to string failed with input %d and output %s",
			    input, output);
		return -1;
	}
	input = -1;
	if (time_to_string(input, output, 12) >= 0) {
		test_failed(
			"Time to string failed with input %d, did not return error",
			input);
		return -1;
	}
	free(output);
	test_passed("Time to string passed");
	return 0;
}

int test_distance_to_string()
{
	int input = 12345;
	char *output = malloc(sizeof(char) * 12);
	distance_to_string(&input, output, 10);
	if (strcmp(output, "12.345 km")) {
		test_failed(
			"Distance to string failed with input %d and output %s",
			input, output);
		return -1;
	}
	free(output);
	test_passed("Distance to string passed");
	return 0;
}

int test_aggregate_data_points()
{
	struct training_data data = { RUN, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	struct csv_line input = {
		L"13-01-2024",
		L"j",
		L"48.00",
		L"",
		L"",
		L"",
		L"4",
		L"cleansquat+askelkyykky+loppuun vähän penaa ja dippejä"
	};
	aggregate_data_points(&data, &input);
	struct csv_line input2 = {
		L"14-01-2024", L"j",
		L"85.10",      L"109",
		L"136",	       L"5.222",
		L"5",	       L"olkapäänjumppaus+kädet+keskivartalo"
	};
	aggregate_data_points(&data, &input2);
	struct csv_line input3 = {
		L"16-01-2024", L"j",
		L"35.03",      L"142",
		L"164",	       L"4.93",
		L"4",	       L"juoksu+kävelylenkki:keuhkojenkuntoutusta"
	};
	aggregate_data_points(&data, &input3);
	if (data.time != 10093 || data.amount_time != 3) {
		test_failed("Aggregate data points failed to count time");
		return -1;
	}
	if (data.amount_heart_rate != 2 || data.heart_rate != 251) {
		test_failed("Aggregate data points failed to count heart rate");
		return -1;
	}
	if (data.heart_rate_max != 164) {
		test_failed(
			"Aggregate data points failed to find max heart rate");
		return -1;
	}
	if (data.evaluation != 13 || data.amount_evaluation != 3) {
		test_failed("Aggregate data points failed to count evaluation");
		return -1;
	}
	if (data.distance != 10152 || data.amount_distance != 2) {
		test_failed("Aggregate data points failed to count distance");
		return -1;
	}
	test_passed("Aggregrate data points passed");
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
		tests_partly_passed(
			"Training utility tests completed, %d/%d tests passed",
			passed, total);
	}
	if (passed == total) {
		test_passed("All training utility tests passed.");
	}
	return 0;
}
