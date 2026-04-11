// file: arena.h
// vim:fileencoding=utf-8:ft=c:tabstop=2
// This is free and unencumbered software released into the public domain.
//
// Author: R.F. Smith <rsmith@xs4all.nl>
// SPDX-License-Identifier: Unlicense
// Created: 2023-04-23T22:07:59+0200
// Last modified: 2026-04-03T15:08:23+0200

// Inspired by: https://nullprogram.com/blog/2023/09/27/

#pragma once

#include <stdint.h>    // for uint8_t
#include <stddef.h>    // for size_t, ptrdiff_t, alignof
#include <stdalign.h>  // for alignof

typedef struct {
  uint8_t *begin;
  ptrdiff_t current_offset;
  ptrdiff_t length;
} Arena;

// Predefined arena sizes.
#define ARENA_SMALL 4096        // 4 kiB, one page on most systems
#define ARENA_DEFAULT 1048576   // 1 MiB
#define ARENA_LARGE 16777216    // 4096 pages on most systems

#ifdef __cplusplus
extern "C" {
#endif

// Create an arena of the given “length”. If “length” is ≤0, ARENA_DEFAULT is used.
extern Arena arena_create(ptrdiff_t length);
// Return the remaining space in bytes in the arena.
extern ptrdiff_t arena_remaining(Arena *arena);
// Allocate from the arena. Best to use arena_new instead.
extern void *arena_alloc(Arena *arena, ptrdiff_t size, ptrdiff_t count,
                         ptrdiff_t align);
// Allocate “n” objects of type “t” from arena “a”.
#define arena_new(a, t, n) (t *)arena_alloc(a, sizeof(t), n, alignof(t))
// Destroy the arena and make it invalid.
extern void arena_destroy(Arena *arena);
// Empty out the arena.
extern void arena_empty(Arena *arena);

#ifdef __cplusplus
}
#endif

#ifdef ARENA_IMPLEMENTATION
#include <assert.h>
#include <stdio.h>      // for printf
#include <stdint.h>     // for uintptr_t
#include <stddef.h>     // for ptrdiff_t
#include <stdlib.h>     // for abort
#include <string.h>     // for memset
#ifdef _WIN32
#include <memoryapi.h>
#define MAP_FAILED ((void *)-1)
#else
#include <sys/mman.h>   // for mmap, munmap
#endif

Arena arena_create(ptrdiff_t length)
{
  Arena arena = {0};
  if (length <= 0) {
    length = ARENA_DEFAULT;
  }
#ifdef _WIN32
  arena.begin = VirtualAlloc(0, length, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
  if (arena.begin == 0) {
    arena.begin = MAP_FAILED;
  }
#else
  arena.begin = mmap(0, length, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
#endif
  if (arena.begin == MAP_FAILED) {
    fprintf(stderr, "arena allocation of size %td failed\n", length);
    abort();
  }
  arena.current_offset = 0;
  arena.length = length;
  return arena;
}

ptrdiff_t arena_remaining(Arena *arena)
{
  assert(arena != 0);
  return arena->length - arena->current_offset;
}

void *arena_alloc(Arena *arena, ptrdiff_t size, ptrdiff_t count, ptrdiff_t align)
{
  assert(arena != 0);
  assert(size > 0);
  assert(count > 0);
  ptrdiff_t padding = -arena->current_offset & (align - 1);
  ptrdiff_t remaining = arena->length - arena->current_offset - padding;
  if (count > remaining / size) {
    fprintf(stderr, "arena %p exhausted; %td items of %td bytes requested, %td available\n",
            (void *)arena, count, size, remaining / size);
    abort();
  }
  void *rv = arena->begin + arena->current_offset + padding;
  arena->current_offset += padding + count * size;
  return memset(rv, 0, count * size);
}

void arena_destroy(Arena *arena)
{
  assert(arena != 0);
  int rv;
#ifdef _WIN32
  rv = VirtualFree(arena->begin, 0, MEM_RELEASE);
  if (rv == 0) {
    rv = -1;
  }
#else
  rv = munmap(arena->begin, arena->length);
#endif
  if (rv == -1) {
    fprintf(stderr, "destroying arena %p failed\n", (void *)arena);
  }
  Arena empty = {0};
  *arena = empty;
}

void arena_empty(Arena *arena)
{
  assert(arena != 0);
  // Clear all the used memory.
  memset(arena->begin, 0, arena->current_offset);
  // Reset the use counter.
  arena->current_offset = 0;
}

#endif // ARENA_IMPLEMENTATION
