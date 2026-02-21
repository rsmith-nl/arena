CFLAGS = -pipe -std=c11 -Wall -Wextra -Wstrict-prototypes -Wpedantic \
                -Wshadow -Wmissing-field-initializers -Wpointer-arith

check:  ## checks if the code builds cleanly.
	$(CC) $(CFLAGS) -c arena.c
	rm -f *.o

atest: arena.c arena.h  ## builds the test program.
	$(CC) $(CFLAGS) -o atest atest.c arena.c

.PHONY: clean
clean:
	rm -f *.o atest

.PHONY: style
style:  ## Reformat source code using astyle.
	astyle -n *.c *.h

.PHONY: tidy
tidy:  ## Check the code with clang-tidy
	clang-tidy19 --quiet *.c *.h 2>/dev/null

.PHONY: help
help:  ## List available commands
	@echo "make targets:"
	@echo
	@sed -n -e '/##/s/:.*\#\#/\t/p' Makefile
