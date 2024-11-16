#ifndef UTIL_H
#define UTIL_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define TOKEN_RAW_CAPACITY 1024 * 10 // 10KB of raw string for the token

#define fail_if(expr, ...) if (expr) { fprintf(stderr, ##__VA_ARGS__); exit(1); }
#define str_eq(a, b) (strcmp(a, b) == 0)
#endif // UTIL_H
