# vim:fileencoding=utf-8:ft=make
# Use as many jobs as the computer has cores.
.MAKEFLAGS: -j C

CFLAGS = -pipe -std=c11 -Wall -Wextra -Wstrict-prototypes -Wpedantic \
                -Wshadow -Wmissing-field-initializers -Wpointer-arith

all: atest single_header/arena.h

atest: atest.c arena.c arena.h ## builds the test program (default).
	$(CC) $(CFLAGS) -o atest atest.c arena.c

single_header/arena.h: arena.c arena.h  ## Build single header library (POSIX only).
	cp arena.h single_header/arena.h
	echo "" >>single_header/arena.h
	echo "#ifdef ARENA_IMPLEMENTATION" >>single_header/arena.h
	tail -n +11 arena.c >>single_header/arena.h
	echo "" >>single_header/arena.h
	echo "#endif // ARENA_IMPLEMENTATION" >>single_header/arena.h

.PHONY: clean
clean:
	rm -f *.o atest single_header/arena.h

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
