/*  file: arena.h
 *  vim:fileencoding=utf-8:fdm=marker:ft=c
 *
 *  Copyright © 2023 R.F. Smith <rsmith@xs4all.nl>
 *  SPDX-License-Identifier: MIT
 *  Created: 2023-04-23T22:07:59+0200
 *  Last modified: 2023-07-17T20:29:10+0200
 */

#pragma once

#include <sys/types.h> /* for uint8_t and size_t */

typedef struct arena Arena;

#ifdef __cplusplus
extern "C" {
#endif

void arena_create(Arena *arena, size_t length);
size_t arena_remaining(Arena *arena);
void *arena_allocate(Arena *arena, size_t size);
void arena_destroy(Arena *arena);

#ifdef __cplusplus
extern "C" {
#endif
/* EOF */
