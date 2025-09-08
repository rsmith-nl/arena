CFLAGS = -pipe -std=c11 -fPIC -g3 -Wall -Wextra -Wstrict-prototypes -Wpedantic \
                -Wshadow-all -Wmissing-field-initializers -Wpointer-arith

check:
	$(CC) $(CFLAGS) -c arena.c
	clang-tidy19 --quiet *.c *.h 2>/dev/null
	rm -f *.o

test:
	$(CC) $(CFLAGS) -o arena_test arena_test.c arena.c

.PHONY: clean
clean:
	rm -f *.o arena_test
