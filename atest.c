// file: atest.c
// vim:fileencoding=utf-8:ft=c:tabstop=2
// This is free and unencumbered software released into the public domain.
//
// Author: R.F. Smith <rsmith@xs4all.nl>
// SPDX-License-Identifier: Unlicense
// Created: 2025-09-08 20:48:54 +0200
// Last modified: 2025-09-08T22:14:54+0200

#include "arena.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#define ARENA_MAGIC 0x4152454e

// For condition testing, e.g. a == b.
#define test(exp) \
  do \
    if (exp) { \
      printf("\033[0;32mPASSED:\033[0m " #exp "\n"); \
    } else { \
      printf("\033[1;31mFAILED:\033[0m " #exp "\n"); \
    } \
  while (0)


int main(int argc, char *argv[]) {
  (void)argc;
  (void)argv;
  Arena std = arena_create(4096);
  puts("After creating the arena.");
  test(std.magic == ARENA_MAGIC);
  test(std.current_offset == 0);
  test(std.length == 4096);
  char *fname = "arena.c";
  ptrdiff_t size = 1024;
  FILE *f = fopen(fname, "r");
  if (f) {
    fread(arena_new(&std, char, size), size, 1, f);
    fclose(f);
    puts("After adding to the arena.");
    test(std.current_offset == size);
    test(arena_remaining(&std) == (4096 - size));
    test(memcmp(std.begin, "// file: arena.c", 16) == 0);
  }
  arena_empty(&std);
  puts("After empying the arena.");
  test(std.current_offset == 0);
  arena_destroy(&std);
  puts("After destroying the arena.");
  test(std.magic == 0);
  test(std.begin == 0);
  test(std.current_offset == 0);
  test(std.length == 0);
  return 0;
}
