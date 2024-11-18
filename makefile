.POSIX:

CFLAGS += -Wall -Wextra -pedantic -std=c89 -I .

example_dir = examples
examples = \
    01-fibonacci-sequence \
    02-repeating-strings \
    03-error-handling

.PHONY: all clean
all: $(examples)

clean:
	$(RM) $(examples) tags

01-fibonacci-sequence: $(example_dir)/01-fibonacci-sequence.c
	$(CC) $(CFLAGS) -o $@ $^

02-repeating-strings: $(example_dir)/02-repeating-strings.c
	$(CC) $(CFLAGS) -o $@ $^

03-error-handling: $(example_dir)/03-error-handling.c
	$(CC) $(CFLAGS) -o $@ $^

tags:
	ctags *.h $(example_dir)/*.c
