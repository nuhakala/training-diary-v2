# Training diary

Rather simple tool for collecting and saving training data into csv-format.

You can compile the program using the given makefile.

The program writes the data into `./data/<current year>.csv` by default, however
it cannot create the directory, so create the directory manually before running.

You can also give the save file as parameter if you don't want to use the
default: `./data-collector.o <custom file>`.

If the file does not exists (either the default file or the given parameter),
the program will create it and write the header there. If the file already
exists, then no header is written (even if the file would be empty).

# Statistics tool

This is another tool to read and parse the data points saved by the data
collector. It reads the same file by default or one can also give the file as
parameter to it (similarly as to data collector).

This tool will parse and print the training data by category and by month. It
will also print the statistics for whole year.

# Windows support

I have compiled and tested this on Windows using w64devkit. It compiled and
worked fine except for unicode (characters ä and ö are not working, most likely
rest of unicode don't work either). Unicode characters do not work properly, but
I assume that it is a feature of Windows terminal. Unicode characters work fine
when using Linux.
