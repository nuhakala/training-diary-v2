#!/bin/bash

collect() {
	FILE=./data_collector.o
	if test -f "$FILE"; then
		./data_collector.o $2
	else
		make build
		./data_collector.o $2
	fi
}

statistics()
{
	FILE=./statistics_tool.o
	if test -f "$FILE"; then
		./statistics_tool.o $1
	else
		make build
		./statistics_tool.o $1
	fi
}

usage()
{
	echo "Usage:"
	echo "Collect data: collect <optional:file_name>"
	echo "Print statistics: stat <optional:file_name>"
}

source_dir=./
if [ "$1" = "collect" ]; then
	cd ${source_dir}
	collect $2
elif [ "$1" = "stat" ]; then
	cd ${source_dir}
	statistics $2
else
	usage
fi

