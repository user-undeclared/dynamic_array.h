.POSIX:

CFLAGS += -Wall -Wextra -pedantic -std=c89 -I .

example_dir = examples

all: 01-fibonacci-sequence 02-repeating-strings

clean:
	$(RM) 01-fibonacci-sequence 02-repeating-strings

01-fibonacci-sequence: $(example_dir)/01-fibonacci-sequence.c
	$(CC) $(CFLAGS) -o $@ $^

02-repeating-strings: $(example_dir)/02-repeating-strings.c
	$(CC) $(CFLAGS) -o $@ $^
