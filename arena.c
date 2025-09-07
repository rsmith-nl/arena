// file: arena.c
// vim:fileencoding=utf-8:ft=c:tabstop=2
// This is free and unencumbered software released into the public domain.
//
// Author: R.F. Smith <rsmith@xs4all.nl>
// SPDX-License-Identifier: Unlicense
// Created: 2023-04-23T22:08:02+0200
// Last modified: 2025-09-08T00:12:46+0200

#include "arena.h"
#include "logging.h"
#include <stdio.h>      // for printf
#include <stdint.h>     // for uintptr_t
#include <stddef.h>     // for ptrdiff_t
#include <string.h>     // for memset
#include <sys/mman.h>   // for mmap, munmap

// In [9]: '0x' + ''.join([hex(ord(j))[2:] for j in "AREN"])
// Out[9]: '0x4152454e'
#define ARENA_MAGIC 0x4152454e

Arena arena_create(ptrdiff_t length)
{
  Arena arena = {0};
  // Default length 1 MiB.
  if (length <= 0) {
    length = 1048576;
  }
  arena.magic = ARENA_MAGIC;
  // using mmap for memory should return a page aligned address.
  arena.begin =
    mmap(0, length, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
  if (arena.begin == MAP_FAILED) {
    error("arena allocation of size %td failed\n", length);
  }
  info("arena; %td bytes allocated", length);
  arena.cur = 0;
  arena.length = length;
  return arena;
}

ptrdiff_t arena_remaining(Arena *arena)
{
  if (arena == 0) {
    error("null pointer used for arena\n");
  }
  if (arena->magic != ARENA_MAGIC) {
    error("invalid arena %p; magic %d\n", (void*)arena, arena->magic);
  }
  return arena->length - arena->cur;
}

void *arena_alloc(Arena *arena, ptrdiff_t size, ptrdiff_t count, ptrdiff_t align)
{
  if (arena == 0) {
    error("null pointer used for arena\n");
  }
  if (arena->magic != ARENA_MAGIC) {
    error("invalid arena %p; magic %d\n", (void *)arena, arena->magic);
  }
  ptrdiff_t padding = -arena->cur & (align - 1);
  ptrdiff_t remaining = arena->length - arena->cur - padding;
  if (count > remaining/size) {
    error("arena %p exhausted; %td items of %td bytes requested, %td available\n",
          (void *)arena, count, size, remaining/size);
  }
  void *rv = arena->begin + arena->cur + padding;
  arena->cur += padding + count * size;
  return memset(rv, 0, count * size);
}

void arena_destroy(Arena *arena)
{
  if (arena == 0) {
    error("null pointer used for arena ignored\n");
    return;
  }
  if (arena->magic != ARENA_MAGIC) {
    error("invalid arena %p; magic %d ignored\n", (void *)arena, arena->magic);
    return;
  }
  int rv = munmap(arena->begin, arena->length);
  if (rv == -1) {
    error("destroying arena %p failed\n", (void *)arena);
  }
  Arena empty = {0};
  *arena = empty;
}

void arena_empty(Arena *arena)
{
  if (arena == 0) {
    error("null pointer used for arena ignored\n");
  }
  if (arena->magic != ARENA_MAGIC) {
    error("invalid arena %p; magic %d ignored\n", (void *)arena, arena->magic);
  }
  // Clrear memory to the current offset
  memset(arena->begin, 0, arena->cur);
  // Set the offset to 0.
  arena->cur = 0;
}
