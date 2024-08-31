//  file: arena.h
//  vim:fileencoding=utf-8:fdm=marker:ft=c
//
//  Copyright © 2023 R.F. Smith <rsmith@xs4all.nl>
//  SPDX-License-Identifier: MIT
//  Created: 2023-04-23T22:07:59+0200
//  Last modified: 2024-08-31T22:45:30+0200

#pragma once

typedef struct {
  size_t magic;  // magic number to identify an arena
  size_t length;
  size_t used;
  uint8_t *storage;
} Arena;

#ifdef __cplusplus
extern "C" {
#endif

extern void arena_create(Arena *arena, size_t length);
extern size_t arena_remaining(Arena *arena);
extern void *arena_allocate(Arena *arena, size_t size);
extern void arena_destroy(Arena *arena);

#ifdef __cplusplus
extern "C" {
#endif
