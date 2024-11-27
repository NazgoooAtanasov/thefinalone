#ifndef AST_H
#define AST_H

#include <stdint.h>
#include "core/toker.h"
#include "position.h"
#include "vendor/result.h"
#include "vendor/assoc_array.h"

struct node_metadata {
  Position start;
  Position end;
}; 

#define AST_MAX_ARGUMENTS 6
#define AST_MAX_BODY_STATEMENTS 1024
#define FUNCTION_DEF_CAPACITY 1024

typedef enum {
  Type_UNKNOWN,
  Type_i32,
  Type_i64,
  Type_void,
  Type_Capacity
} Type;

typedef enum StatementType {
  StatementType_UNKNOWN,
  StatementType_VariableAssign,
  StatementType_FunctionCall,
} StatementType;

typedef struct {
  char name[TOKEN_RAW_CAPACITY];
  Type type;
  char literal_value[TOKEN_RAW_CAPACITY];
  struct node_metadata _meta;
  uint32_t offset; // this is set by translation to the specified target. check more in target.(h/c)
} VariableAssignStatement;

typedef struct {
  char name[TOKEN_RAW_CAPACITY];
  bool is_intrinsic;
  char arguments[AST_MAX_ARGUMENTS][TOKEN_RAW_CAPACITY];
  uint32_t arguments_count;
  struct node_metadata _meta;
} FunctionCallStatement;

typedef struct {
  StatementType type;
  void* statement;
  struct node_metadata _meta;
} StatementNode;

typedef struct {
  StatementNode statements[AST_MAX_BODY_STATEMENTS];
  uint32_t statements_count;
  AssocArray variables; // assigned and filled in on semantic check
  struct node_metadata _meta;
} BodyNode;

typedef struct {
  char name[TOKEN_RAW_CAPACITY];
  Type type;
} Argument;

typedef struct {
  Argument args[AST_MAX_ARGUMENTS];
  uint32_t arguments_count;
  struct node_metadata _meta;
} ArgumentsNode;

typedef struct {
  char name[TOKEN_RAW_CAPACITY];
  Type return_type;
  ArgumentsNode* arguments;
  BodyNode* body;
  struct node_metadata _meta;
} FunctionNode;

typedef struct {
  FunctionNode* functions[FUNCTION_DEF_CAPACITY];
  uint32_t function_count;
  struct node_metadata _meta;
} RootNode;

typedef struct {
  RootNode* root;
} AST;

#ifdef COMPILER_DEBUG
const char* ast_type_to_str(Type type);
const char* ast_statement_type_to_str(StatementType type);
#endif

AST* parse_ast(Tokens* tokens);

#endif // AST_H
