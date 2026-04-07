// file: atest.c
// vim:fileencoding=utf-8:ft=c:tabstop=2
// This is free and unencumbered software released into the public domain.
//
// Author: R.F. Smith <rsmith@xs4all.nl>
// SPDX-License-Identifier: Unlicense
// Created: 2025-09-08 20:48:54 +0200
// Last modified: 2026-04-08T00:00:59+0200

#define ARENA_IMPLEMENTATION
#include "single_header/arena.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#define ARENA_MAGIC 0x4152454e

#define BOLD_WHITE "\033[1;37m"
#define CYAN "\033[0;36m"
#define GREEN "\033[0;32m"
#define BOLD_GREEN "\033[1;32m"
#define PURPLE "\033[0;35m"
#define BOLD_RED "\033[1;31m"
#define RED "\033[0;31m"
#define YELLOW "\033[0;33m"
#define BOLD_YELLOW "\033[1;33m"
#define RESET "\033[0m"  // No Color

// For condition testing, e.g. a == b.
#define test(exp) \
  do \
    if (exp) { \
      printf("\033[0;32mPASSED:\033[0m " #exp "\n"); \
    } else { \
      printf("\033[1;31mFAILED:\033[0m " #exp "\n"); \
      failcount++; \
    } \
  while (0)


int main(int argc, char *argv[])
{
  (void)argc;
  (void)argv;
  int failcount = 0;
  puts(PURPLE"Starting the tests."RESET);
  Arena std = arena_create(4096);
  puts(CYAN"After creating the arena."RESET);
  test(std.current_offset == 0);
  test(std.length == 4096);
  char *fname = "arena.c";
  ptrdiff_t size = 1024;
  FILE *f = fopen(fname, "r");
  if (f) {
    fread(arena_new(&std, char, size), size, 1, f);
    fclose(f);
    puts(CYAN"After adding to the arena."RESET);
    test(std.current_offset == size);
    test(arena_remaining(&std) == (4096 - size));
    test(memcmp(std.begin, "// file: arena.c", 16) == 0);
  }
  arena_empty(&std);
  puts(CYAN"After empying the arena."RESET);
  test(std.current_offset == 0);
  arena_destroy(&std);
  puts(CYAN"After destroying the arena."RESET);
  test(std.begin == 0);
  test(std.current_offset == 0);
  test(std.length == 0);
  puts(PURPLE"*** Result ***"RESET);
  if (failcount == 0) {
    puts(GREEN"+++ All tests PASSED! +++"RESET);
  } else {
    printf("\033[1;31m--- %d failing tests.---\033[0m\n", failcount);
  }
  return failcount;
}
