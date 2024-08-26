all:
	gcc -Wall -DDEBUG main.c csv_utilities.c training_utilities.c -o main
	./main

build:
	gcc -Wall main.c csv_utilities.c training_utilities.c -o main
	./main
