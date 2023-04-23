/*  file: arena.h
 *  vim:fileencoding=utf-8:fdm=marker:ft=c
 *
 *  Copyright © 2023 R.F. Smith <rsmith@xs4all.nl>
 *  SPDX-License-Identifier: MIT
 *  Created: 2023-04-23T22:07:59+0200
 *  Last modified: 2023-04-23T22:54:19+0200
 */

#pragma once

typedef struct {
    size_t length;
    size_t used;
    uint8_t *storage;
} arena_t;

#ifdef __cplusplus
extern "C" {
#endif

void arena_create(arena_t *arena, size_t length);
void *arena_allocate(arena_t *arena, size_t size);
void arena_destroy(arena_t *arena);

#ifdef __cplusplus
extern "C" {
#endif
/* EOF */
