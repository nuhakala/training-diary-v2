all:
	gcc -Wall -DDEBUG statistics_tool.c csv_utilities.c training_utilities.c utf8.c windows_types.c -o statistics_tool.o
	gcc -Wall -DDEBUG data_collector.c csv_utilities.c training_utilities.c input_utilities.c utf8.c windows_types.c -o data_collector.o

build:
	gcc -Wall statistics_tool.c csv_utilities.c training_utilities.c utf8.c windows_types.c -o statistics_tool.o
	gcc -Wall data_collector.c csv_utilities.c training_utilities.c input_utilities.c utf8.c windows_types.c -o data_collector.o

test:
	gcc -Wall -DDEBUG training_utilities_test.c training_utilities.c -o training_utilities_test.o
	# run valgrind to see that there is no leaks in the tested code either
	valgrind --leak-check=full ./training_utilities_test.o
	# ./training_utilities_test.o

clear:
	rm ./*.o
