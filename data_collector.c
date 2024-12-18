#include "debug_prints.h"
#include "training_utilities.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "csv_utilities.h"
#include "input_utilities.h"

int main(int argc, char *argv[])
{
	char *buffer;
	size_t bufsize = BUF_SIZE;
	buffer = (char *)malloc(bufsize * sizeof(char));
	if( buffer == NULL)
	{
		perror("Unable to allocate buffer");
		exit(1);
	}
	read_first_n(stdin, buffer, bufsize, "type something: ");
	printf("result: %s\n", buffer);
	read_last_n(stdin, buffer, bufsize, "type something: ");
	printf("result: '%s'\n", buffer);
	free(buffer);

	return 0;
}
