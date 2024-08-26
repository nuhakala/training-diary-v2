all:
	gcc -Wall -DDEBUG statistics_tool.c csv_utilities.c training_utilities.c -o statistics_tool.o
	./statistics_tool.o

build:
	gcc -Wall statistics_tool.c csv_utilities.c training_utilities.c -o statistics_tool.o
	./statistics_tool.o

clear:
	rm ./*.o
