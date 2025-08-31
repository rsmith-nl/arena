Arena allocation for C
######################

:date: 2025-08-31
:tags: C programming, memory management, public domain
:author: Roland Smith

.. Last modified: 2025-08-31T10:17:20+0200
.. vim:spelllang=en

Introduction
============

This code was inspired by and based on `Arena allocator tips and tricks`_ by Chris Wellons.

.. _Arena allocator tips and tricks: https://nullprogram.com/blog/2023/09/27/

Error handling is done using the ``error`` macro from ``logging.h`` (which was
inspired by Python).

For me, using arenas has been a *huge* improvement over ``malloc()``/``free()``.

Implementation
==============

The allocation function uses ``mmap`` with ``flags`` ``MAP_ANON|MAP_PRIVATE``.
This means that even is you *request* a big chunk of memory, only the parts
you *use* are added to your process.


Usage
=====

The included Makefile can build a static library or a dynamically linked
library.
But for the sake of simplicity I encourage you to jusr copy the source files
(``arena.h``, ``arena.c`` and ``logging.h``) in to your project.
