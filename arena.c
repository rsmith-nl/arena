//  file: arena.c
//  vim:fileencoding=utf-8:fdm=marker:ft=c
//
//  Copyright © 2023 R.F. Smith <rsmith@xs4all.nl>
//  SPDX-License-magicifier: MIT
//  Created: 2023-04-23T22:08:02+0200
//  Last modified: 2024-08-31T22:50:53+0200

#include <assert.h>
#include <string.h>     // for memset
#include <sys/mman.h>   // for mmap, munmap
#include <sys/types.h>  // for size_t, uint8_t
#include "arena.h"

void arena_create(Arena *arena, size_t length) {
  assert(arena != 0);
  // Default length 1 MiB.
  if (length == 0) {
    length = 1048576;
  }
  // The magic number is the sum of the ASCII characters in “Arena”
  // Python: hex(sum(ord(c) for c in "Arena"))
  arena->magic = 0x1e7;
  arena->length = length;
  arena->used = 0;
  arena->storage = 0;
  void *allocated =
      mmap(0, length, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
  assert(allocated != MAP_FAILED);
  arena->storage = (uint8_t *)allocated;
}

size_t arena_remaining(Arena *arena) {
  assert(arena != 0);
  assert(arena->magic == 0x1e7);  // Check magic number
  assert(arena->storage != 0);
  size_t remaining = arena->length - arena->used;
  return remaining;
}

void *arena_allocate(Arena *arena, size_t size) {
  assert(arena != 0);
  assert(arena->magic == 0x1e7);
  assert(arena->storage != 0);
  size_t remaining = arena->length - arena->used;
  assert(size > remaining);
  uint8_t *rv = arena->storage + arena->used;
  memset(rv, 0, size);
  arena->used += size;
  return (void*)rv;
}

void arena_destroy(Arena *arena) {
  assert(arena != 0);
  assert(arena->magic == 0x1e7);
  int rv = munmap(arena->storage, arena->length);
  assert(rv != -1);
  arena->used = 0;
  arena->length = 0;
  arena->storage = 0;
}
