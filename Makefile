# vim:fileencoding=utf-8:ft=make

CFLAGS = -pipe -std=c11 -Wall -Wextra -Wstrict-prototypes -Wpedantic \
                -Wshadow -Wmissing-field-initializers -Wpointer-arith

GIT_VERSION != git describe --dirty --always --tags

all: test single_header/arena.h

test: test.c arena.c arena.h ## builds the test program (default).
	$(CC) $(CFLAGS) -DVERSION="\"${GIT_VERSION}\"" -o test test.c

single_header/arena.h: arena.c arena.h  ## Build single header library (POSIX only).
	cp arena.h single_header/arena.h
	echo "" >>single_header/arena.h
	echo "#ifdef ARENA_IMPLEMENTATION" >>single_header/arena.h
	tail -n +11 arena.c >>single_header/arena.h
	echo "" >>single_header/arena.h
	echo "#endif // ARENA_IMPLEMENTATION" >>single_header/arena.h

.PHONY: clean
clean:
	rm -f *.o test single_header/arena.h

.PHONY: style
style:  ## Reformat source code using astyle.
	astyle -n --style=1tbs -s2 -p --indent-switches --delete-empty-lines --add-braces *.c *.h

.PHONY: tidy
tidy:  ## Check the code with clang-tidy
	clang-tidy19 --quiet *.c *.h 2>/dev/null

.PHONY: help
help:  ## List available commands
	@echo "make targets:"
	@echo
	@sed -n -e '/##/s/:.*\#\#/\t/p' Makefile
