// file: logging.h
// vim:fileencoding=utf-8:ft=c:tabstop=2
// This is free and unencumbered software released into the public domain.
//
// Author: R.F. Smith <rsmith@xs4all.nl>
// SPDX-License-Identifier: Unlicense
// Created: 2025-08-04 00:49:24 +0200
// Last modified: 2025-08-28T22:15:16+0200

#pragma once

#include <stdio.h>    // for fprintf, stderr, fflush
#include <stdlib.h>   // for abort

#undef error
#ifndef NDEBUG
#define error(...) \
  fprintf(stderr, "# ERROR %s, line %i: ", __FILE__, __LINE__); \
  fprintf(stderr, __VA_ARGS__); \
  fprintf(stderr, "\n"); \
  fflush(stderr); \
  abort()
#else
#define error(...) \
  fprintf(stderr, "# ERROR: "); \
  fprintf(stderr, __VA_ARGS__); \
  fprintf(stderr, "\n"); \
  fflush(stderr); \
  abort()
#endif  // NDEBUG

#undef debug
#ifndef NDEBUG
#define debug(...) \
  fprintf(stderr, "# DEBUG %s, line %i: ", __FILE__, __LINE__); \
  fprintf(stderr, __VA_ARGS__); \
  fprintf(stderr, "\n"); \
  fflush(stderr)
#else
#define debug(...) (void)0
#endif  // NDEBUG

#undef warn
#ifndef NDEBUG
#define warn(...)                                            \
  fprintf(stderr, "# WARNING %s, line %i: ", __FILE__, __LINE__); \
  fprintf(stderr, __VA_ARGS__); \
  fprintf(stderr, "\n"); \
  fflush(stderr)
#else
#define warn(...) \
  fprintf(stderr, "# WARNING: "); \
  fprintf(stderr, __VA_ARGS__); \
  fprintf(stderr, "\n"); \
  fflush(stderr)
#endif  // NDEBUG

#undef info
#ifndef NDEBUG
#define info(...) \
  fprintf(stderr, "# INFO %s, line %i: ", __FILE__, __LINE__); \
  fprintf(stderr, __VA_ARGS__); \
  fprintf(stderr, "\n"); \
  fflush(stderr)
#else
#define info(...) (void)0
#endif  // NDEBUG
