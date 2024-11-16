#ifndef AST_H
#define AST_H

#include <string.h>
#include "core/toker.h"

typedef enum {
  Type_i32
} Type;

typedef struct {} BodyNode;
typedef struct {} ArgumentsNode;

typedef struct {
  char name[TOKEN_RAW_CAPACITY];
  Type return_type;
  ArgumentsNode* arguments;
  BodyNode* body;
} FunctionNode;

#define FUNCTION_DEF_CAPACITY 1024
typedef struct {
  FunctionNode* functions[FUNCTION_DEF_CAPACITY];
  uint32_t function_count;
} RootNode;

typedef struct {
  RootNode* root;
} AST;

AST* parse_ast(Tokens* tokens);

#endif // AST_H
