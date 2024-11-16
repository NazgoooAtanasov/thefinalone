// Copyright 2024 Atanas Atanasov <naskodragomirov@gmail.com>
 
// Permission is hereby granted, free of charge,
// to any person obtaining a copy of this software and associated documentation files (the “Software”), 
// to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, 
// and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 
// THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
// WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

/*
  string_builder - v0.0.1 - Public domain - https://github.com/NazgoooAtansov/string_builder.h

  Simple, bare but yet usable string builder for C projects
  **This does not allocate any memory on the heap so no need manual memory cleanup**

  # Simple Example

  ```c
  #define STRING_BUILDER_IMPLEMENTATION
  #include "string_builder.h"
  
  #include <stdio.h>

  int main(void) {
    StringBuilder sb = {0};
    const char* value = "Hello world";
    string_builder_append_str(&sb, value, 11);
    printf("%s\n", sb.string);
  }
  ```
  
  # Configuration
  The string builder's internal container, holding all of the characters (the string) has a default size of ~8KB (1024 * 8) defined with `STRING_BUILDER_CHAR_CAPACITY`.
  That capacity can be chaged by defining the `STRING_BUILDER_CHAR_CAPACITY` right after `STRING_BUILDER_IMPLEMENTATION`

  ```c
  #define STRING_BUILDER_IMPLEMENTATION
  #define STRING_BUILDER_CHAR_CAPACITY 1024 * 10 // for ~10KB of memory
  #include "string_builder.h"
  
  #include <stdio.h>

  int main(void) {
    StringBuilder sb = {0};
    const char* value = "Hello world";
    string_builder_append_str(&sb, value, 11);
    printf("%s\n", sb.string);
  }
  ```
*/

#ifndef STRING_BUILDER_H
#define STRING_BUILDER_H

#ifndef STRING_BUILDER_DEF
#define STRING_BUILDER_DEF
#endif

#ifndef STRING_BUILDER_NO_STDLIB
#include <stdlib.h>
#define SIZE_T size_t
#endif

#ifndef STRING_BUILDER_CHAR_CAPACITY
#define STRING_BUILDER_CHAR_CAPACITY 1024 * 8 // 8KB initial storage
#endif

#ifndef STRING_BUILDER_ASSERT
#include <assert.h>
#define STRING_BUILDER_ASSERT(...) assert(__VA_ARGS__)
#endif

typedef struct {
  char string[STRING_BUILDER_CHAR_CAPACITY];
  SIZE_T count;
} StringBuilder;

STRING_BUILDER_DEF void string_builder_null(StringBuilder* sb);
STRING_BUILDER_DEF void string_builder_append_chr(StringBuilder* sb, const char value);
STRING_BUILDER_DEF void string_builder_append_str(StringBuilder* sb, const char* value, SIZE_T len);

#endif // end of header file STRING_BUILDER_H

#ifdef STRING_BUILDER_IMPLEMENTATION

STRING_BUILDER_DEF void string_builder_null(StringBuilder* sb) {
  sb->count = 0;
}

STRING_BUILDER_DEF void string_builder_append_chr(StringBuilder* sb, const char value) {
  STRING_BUILDER_ASSERT(sb->count + 1 <= STRING_BUILDER_CHAR_CAPACITY && "STRING_BUILDER_ASSERT out of bounds");
  sb->string[sb->count++] = value;
}

STRING_BUILDER_DEF void string_builder_append_str(StringBuilder* sb, const char* value, SIZE_T len) {
  for (SIZE_T i = 0; i < len; ++i) {
    string_builder_append_chr(sb, value[i]);
  }
}

#endif // end of implementation file STRING_BUILDER_IMPLEMENTATION
