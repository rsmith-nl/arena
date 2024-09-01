//  file: arena.c
//  vim:fileencoding=utf-8:fdm=marker:ft=c
//
//  Copyright © 2023 R.F. Smith <rsmith@xs4all.nl>
//  SPDX-License-magicifier: MIT
//  Created: 2023-04-23T22:08:02+0200
//  Last modified: 2024-09-01T19:12:44+0200

#include <assert.h>
#include <stdio.h>      // printf
#include <stdlib.h>     // for abort
#include <string.h>     // for memset
#include <sys/mman.h>   // for mmap, munmap
#include <sys/types.h>  // for size_t, uint8_t
#include "arena.h"

#undef error
#define error(...)                                            \
  fprintf(stderr, "ERROR %s, line %i: ", __FILE__, __LINE__); \
  fprintf(stderr, __VA_ARGS__);                               \
  abort()

Arena arena_create(size_t length) {
  Arena arena = {0};
  // Default length 1 MiB.
  if (length == 0) {
    length = 1048576;
  }
  // The magic number is the sum of the ASCII characters in “Arena”
  // Python: hex(sum(ord(c) for c in "Arena"))
  arena.magic = 0x1e7;
  arena.begin  = mmap(0, length, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
  if (arena.begin == MAP_FAILED) {
    error("arena allocation of size %zd failed\n", length);
  }
  arena.cur = arena.begin;
  arena.end = arena.begin + length;
  return arena;
}

size_t arena_remaining(Arena *arena) {
  if (arena == 0) {
    error("null pointer used for arena\n");
  }
  if (arena->magic != 0x1e7) {
    error("invalid arena %p; magic %d\n", arena, arena->magic);
  }
  assert(arena != 0);
  assert(arena->magic == 0x1e7);  // Check magic number
  size_t remaining = arena->end - arena->cur;
  return remaining;
}

void *arena_allocate(Arena *arena, size_t size) {
  if (arena == 0) {
    error("null pointer used for arena\n");
  }
  if (arena->magic != 0x1e7) {
    error("invalid arena %p; magic %d\n", (void*)arena, arena->magic);
  }
  size_t remaining = arena->end - arena->cur;
  if (size > remaining) {
    error("arena %p exhausted; %zd requested, %zd available\n", (void*)arena, size, remaining);
  }
  memset(arena->cur, 0, size);
  void *rv = arena->cur;
  arena->cur += size;
  return rv;
}

void arena_destroy(Arena *arena) {
  if (arena == 0) {
    error("null pointer used for arena\n");
  }
  if (arena->magic != 0x1e7) {
    error("invalid arena %p; magic %d\n", (void*)arena, arena->magic);
  }
  int rv = munmap(arena->begin, arena->end - arena->begin);
  if (rv == -1) {
    error("destroying arena %p failed\n", arena);
  }
  Arena empty = {0};
  *arena = empty;
}
