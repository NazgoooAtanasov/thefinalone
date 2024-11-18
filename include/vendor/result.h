#ifndef RESULT_H
#define RESULT_H

#ifndef RESULT_NO_STDLIB
#include <stdlib.h>
#endif

#ifndef RESULT_NO_STDBOOL
#include <stdbool.h>
#define BOOLEAN bool
#endif

#ifndef RESULT_ASSERT
#include <assert.h>
#define RESULT_ASSERT(...) assert(__VA_ARGS__)
#endif

#ifndef RESULT_DEF
#define RESULT_DEF
#endif

#ifndef RESULT_ERROR_BUFFER_SZ
#define RESULT_ERROR_BUFFER_SZ 1024 * 8 // ~8KB
#endif

typedef struct {
  void* value;
  char error[RESULT_ERROR_BUFFER_SZ];
} Result;

#define result_set_error(result, ...) snprintf((result.error), sizeof(result.error), __VA_ARGS__)

RESULT_DEF Result result_create_empty();
RESULT_DEF void* result_unwrap(Result* result);
RESULT_DEF const char* result_error(Result* result);
RESULT_DEF BOOLEAN result_is_ok(Result* result);
RESULT_DEF BOOLEAN result_is_err(Result* result);
#endif // RESULT_H

#ifdef RESULT_H_IMPLEMENTATION
RESULT_DEF Result result_create_empty() {
  return (Result) {.value = NULL, .error = "\0"};
}

RESULT_DEF void* result_unwrap(Result* result) {
  RESULT_ASSERT(result->value != NULL && "[ERROR]: Unwrapping empty value");
  return result->value;
}

RESULT_DEF const char* result_error(Result* result) {
  return result->error;
}

RESULT_DEF BOOLEAN result_is_ok(Result* result) {
  return result->error[0] == '\0';
}

RESULT_DEF BOOLEAN result_is_err(Result* result) {
  return result->error[0] != '\0';
}
#endif
