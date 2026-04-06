Arena allocation for C
######################

:date: 2025-08-31
:tags: C programming, memory management, public domain
:author: Roland Smith

.. Last modified: 2026-04-07T00:48:36+0200
.. vim:spelllang=en

Introduction
============

This code was inspired by and based on `Arena allocator tips and tricks`_ by Chris Wellons.

.. _Arena allocator tips and tricks: https://nullprogram.com/blog/2023/09/27/

Error handling is done using the ``error`` macro from ``logging.h``

For me, using arenas has been a *huge* improvement over ``malloc()``/``free()``.


Implementation
==============

Virtual memory is used on all platforms.
Even if you *request* a large arena, only the pages that are actually *used*
are added to your process.

POSIX
-----

Creating and destroying an arena is done using ``mmap`` (with flags
``MAP_ANON|MAP_PRIVATE``) and ``munmap``.


Win32
-----

Creating and destroying an arena is done using ``VirtualAlloc`` (with flags
``MEM_COMMIT|MEM_RESERVE``) and ``VirtualFree``.


Usage
=====

Just copy the source files (``arena.h``, ``arena.c``) in to
your project, and hook them up to the build.

The included ``Makefile`` builds the code for testing and can build a test
program with ``make atest``.

Alternatively, you can use this project as a *single header library*.
This variant can be found in the subdirectory ``single_header/arena.h``.
To use the single header library, copy the file ``arena.h`` from
``single_header/`` into your project.
In **one** of the C-files you use the single header library in, you should
define ``ARENA_IMPLEMENTATION`` before including the library::

    #define ARENA_IMPLEMENTATION
    #include "arena.h"
