#ifndef ASSOC_ARRAY_H
#define ASSOC_ARRAY_H

#include <string.h>
#ifndef ASSOC_ARRAY_DEF
#define ASSOC_ARRAY_DEF
#endif

#ifndef ASSOC_ARRAY_MAX_ITEMS
#define ASSOC_ARRAY_MAX_ITEMS 1024
#endif

#ifndef assoc_array_key_t
#define assoc_array_key_t char*
#endif

#ifndef ASSOC_ARRAY_SIZE_T
#include <stddef.h>
#define ASSOC_ARRAY_SIZE_T size_t
#define ASSOC_ARRAY_SIZE_T_MAX 0xFFFFFFFFFFFFFFFF
#endif

#ifndef ASSOC_ARRAY_NO_STDBOOL
#include <stdbool.h>
#define ASSOC_ARRAY_BOOL bool
#endif

typedef struct {
  assoc_array_key_t keys[ASSOC_ARRAY_MAX_ITEMS];
  void* values[ASSOC_ARRAY_MAX_ITEMS];
  ASSOC_ARRAY_SIZE_T count;
} AssocArray;

ASSOC_ARRAY_DEF AssocArray assoc_array_new();
ASSOC_ARRAY_DEF ASSOC_ARRAY_BOOL assoc_array_has(AssocArray* assoc_array, assoc_array_key_t key);
ASSOC_ARRAY_DEF ASSOC_ARRAY_SIZE_T assoc_array_idx(AssocArray* assoc_array, assoc_array_key_t key);
ASSOC_ARRAY_DEF void* assoc_array_get(AssocArray* assoc_array, assoc_array_key_t key);
ASSOC_ARRAY_DEF void assoc_array_drop(AssocArray* assoc_array, assoc_array_key_t key);
ASSOC_ARRAY_DEF void assoc_array_put(AssocArray* assoc_array, assoc_array_key_t key, void* value);

#endif

#ifdef ASSOC_ARRAY_IMPLEMENTATION

ASSOC_ARRAY_SIZE_T assoc_array_find_next_empty(AssocArray* assoc_array) {
  ASSOC_ARRAY_SIZE_T index = 0;
  for (index = 0; index < assoc_array->count; ++index) {
    if (assoc_array->values[index] == NULL) break;
  }

  return index;
}

ASSOC_ARRAY_DEF AssocArray assoc_array_new() {
  AssocArray assoc_array = {0};
  return assoc_array;
}

ASSOC_ARRAY_DEF ASSOC_ARRAY_SIZE_T assoc_array_idx(AssocArray* assoc_array, assoc_array_key_t key) {
  ASSOC_ARRAY_SIZE_T index = 0;
  for (index = 0; index < assoc_array->count; ++index) {
    if (strcmp(assoc_array->keys[index], key) == 0) break;
  }

  if (assoc_array->values[index] != NULL) {
    return index;
  }

  return -1;
}

ASSOC_ARRAY_DEF ASSOC_ARRAY_BOOL assoc_array_has(AssocArray* assoc_array, assoc_array_key_t key) {
  ASSOC_ARRAY_SIZE_T index = 0;
  for (index = 0; index < assoc_array->count; ++index) {
    if (strcmp(assoc_array->keys[index], key) == 0) break;
  }

  if (assoc_array->values[index] != NULL) {
    return true;
  }

  return false;
}

ASSOC_ARRAY_DEF void* assoc_array_get(AssocArray* assoc_array, assoc_array_key_t key) {
  ASSOC_ARRAY_SIZE_T index = 0;
  for (index = 0; index < assoc_array->count; ++index) {
    if (assoc_array->keys[index] == key) break;
  }

  return assoc_array->values[index];
}

ASSOC_ARRAY_DEF void assoc_array_put(AssocArray* assoc_array, assoc_array_key_t key, void* value) {
  ASSOC_ARRAY_SIZE_T index = assoc_array_idx(assoc_array, key);
  if (index == ASSOC_ARRAY_SIZE_T_MAX) {
    index = assoc_array_find_next_empty(assoc_array);
  }

  assoc_array->keys[index] = key;
  assoc_array->values[index] = value;
  assoc_array->count++;
}
#endif
