# Choose whether to use debug flags
# CFLAGS=-Wall -DDEBUG
CFLAGS=-Wall

STATISTICS_TOOL_OBJS=statistics_tool.o csv_utilities.o training_utilities.o utf8.o windows_types.o
TRAINING_TOOL_OBJS=data_collector.o csv_utilities.o training_utilities.o input_utilities.o utf8.o windows_types.o
TEST_OBJS=training_utilities_test.o training_utilities.o

all: statistics training

statistics: $(STATISTICS_TOOL_OBJS)
	gcc $(CFLAGS) $(STATISTICS_TOOL_OBJS) -o statistics_tool.out

training: $(TRAINING_TOOL_OBJS)
	gcc $(CFLAGS) $(TRAINING_TOOL_OBJS) -o data_collector.out

test: $(TEST_OBJS)
	gcc $(CFLAGS) $(TEST_OBJS) -o training_utilities_test.out
	# run valgrind to see that there is no leaks in the tested code either
	valgrind --leak-check=full ./training_utilities_test.out
	./training_utilities_test.out

clear:
	rm ./*.o
