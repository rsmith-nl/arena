CFLAGS = -pipe -std=c11 -fPIC -g3 -Wall -Wextra -Wstrict-prototypes -Wpedantic \
                -Wshadow-all -Wmissing-field-initializers -Wpointer-arith

check:  ## checks if the code builds cleanly.
	$(CC) $(CFLAGS) -c arena.c
	clang-tidy19 --quiet *.c *.h 2>/dev/null
	rm -f *.o

atest:  ## builds the test program.
	$(CC) $(CFLAGS) -o atest arena_test.c arena.c

.PHONY: clean
clean:
	rm -f *.o atest

.PHONY: style
style:  ## Reformat source code using astyle.
	astyle -n *.c *.h

.PHONY: help
help:  ## List available commands
	@echo "make targets:"
	@echo
	@sed -n -e '/##/s/:.*\#\#/\t/p' Makefile
