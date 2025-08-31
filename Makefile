CFLAGS = -pipe -std=c11 -fPIC -g3 -Wall -Wextra -Wstrict-prototypes -Wpedantic \
                -Wshadow-all -Wmissing-field-initializers -Wpointer-arith

check: arena.o
	clang-tidy19 --quiet *.c *.h 2>/dev/null
	rm -f *.o

.c.o:
	$(CC) $(CFLAGS) $(CPPFLAGS) $(HDIRS) -c -o $@ $<

.PHONY: clean
clean:
	rm -f *.o
