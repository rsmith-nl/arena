//  file: arena.h
//  vim:fileencoding=utf-8:fdm=marker:ft=c
//
//  Copyright © 2023 R.F. Smith <rsmith@xs4all.nl>
//  SPDX-License-Identifier: MIT
//  Created: 2023-04-23T22:07:59+0200
//  Last modified: 2024-05-20T09:11:01+0200

#ifndef INCLUDED_ARENA_H
#define INCLUDED_ARENA_H

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

#endif // INCLUDED_ARENA_H
