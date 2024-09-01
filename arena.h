//  file: arena.h
//  vim:fileencoding=utf-8:fdm=marker:ft=c
//
//  Copyright © 2023 R.F. Smith <rsmith@xs4all.nl>
//  SPDX-License-Identifier: MIT
//  Created: 2023-04-23T22:07:59+0200
//  Last modified: 2024-09-01T18:32:41+0200

#pragma once

typedef struct {
  int32_t magic;  // magic number to identify an arena
  uint8_t *begin;
  uint8_t *cur;
  uint8_t *end;
} Arena;

#ifdef __cplusplus
extern "C" {
#endif

extern Arena arena_create(size_t length);
extern size_t arena_remaining(Arena *arena);
extern void *arena_allocate(Arena *arena, size_t size);
extern void arena_destroy(Arena *arena);

#ifdef __cplusplus
extern "C" {
#endif
