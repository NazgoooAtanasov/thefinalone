#ifndef AST_H
#define AST_H

#include <stdint.h>
#include "core/toker.h"

typedef enum {
  Type_i32
} Type;

typedef enum StatementType {
  StatementType_VariableAssign,
  StatementType_UNKNOWN
} StatementType;

typedef struct {
    char name[TOKEN_RAW_CAPACITY];
    Type type;
    const char literal_value[TOKEN_RAW_CAPACITY];
} VariableAssignStatement;

typedef struct {
    StatementType type;
    union {
      VariableAssignStatement variable_assign;
    } statement;
} StatementNode;

#define AST_MAX_BODY_STATEMENTS 1024
typedef struct {
  StatementNode statements[AST_MAX_BODY_STATEMENTS];
  uint32_t statements_count;
} BodyNode;

typedef struct {
    char name[TOKEN_RAW_CAPACITY];
    Type type;
} Argument;

#define AST_MAX_ARGUMENTS 6
typedef struct {
    Argument args[AST_MAX_ARGUMENTS];
    uint32_t arguments_count;
} ArgumentsNode;

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


#ifdef COMPILER_DEBUG
const char* ast_type_to_str(Type type);
#endif

AST* parse_ast(Tokens* tokens);

#endif // AST_H
