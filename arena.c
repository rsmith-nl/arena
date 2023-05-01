/*  file: arena.c
 *  vim:fileencoding=utf-8:fdm=marker:ft=c
 *
 *  Copyright © 2023 R.F. Smith <rsmith@xs4all.nl>
 *  SPDX-License-Identifier: MIT
 *  Created: 2023-04-23T22:08:02 +0200
 *  Last modified: 2023-05-02T00:21:17+0200
 */

#include <stdio.h> /* for fprintf */
#include <stdlib.h> /* for abort*/
#include <string.h> /* for memset */
#include <sys/mman.h> /* for mmap, munmap */
#include "arena.h"

void arena_create(arena_t *arena, size_t length) {
    if (arena == 0) {
        fprintf(stderr, "invalid arena");
        abort();
    }
    if (length == 0) {
        length = 1048576;
    }
    arena->length = length;
    arena->used = 0;
    arena->storage = 0;
    void *allocated = mmap(0, length, PROT_READ|PROT_WRITE, MAP_ANON|MAP_PRIVATE, -1, 0);
    if (allocated == MAP_FAILED) {
        fprintf(stderr, "memort for arena cannot be allocated");
        abort();
    }
    arena->storage = (uint8_t *)allocated;
}

size_t arena_remaining(arena_t *arena) {
    if (arena == 0) {
        fprintf(stderr, "invalid arena");
        abort();
    }
    if (arena->storage == 0) {
        fprintf(stderr, "uninitialized arena");
        abort();
    }
    size_t remaining = arena->length - arena->used;
    return remaining;
}

void *arena_allocate(arena_t *arena, size_t size) {
    size_t remaining = arena_remaining(arena);
    if (size > remaining) {
        fprintf(stderr, "unable to allocate requested size");
        abort();
    }
    uint8_t *rv = arena->storage + arena->used;
    memset(rv, 0, size);
    arena->used += size;
    return rv;
}

void arena_destroy(arena_t *arena) {
    if (arena == 0) {
        fprintf(stderr, "invalid arena");
        abort();
    }
    int rv = munmap(arena->storage, arena->length);
    if (rv == -1) {
        fprintf(stderr, "arena unmap failed");
        abort();
    }
}

/* EOF */
