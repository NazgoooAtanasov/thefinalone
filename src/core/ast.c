#include "core/ast.h"
#include "core/toker.h"
#include "util.h"

#ifdef COMPILER_DEBUG
const char* ast_type_to_str(Type type) {
    switch(type) {
        case Type_i32: return "i32";
        default: return "UNKNOWN";
    }
}
#endif

Type map_type(TokenKeywordKind kind) {
  switch(kind) {
    case TokenKeywordKind_i32Type: return Type_i32;
    default: fail_if(true, "Bail out, unknown type kind.");
  }
}

Type parse_type(Tokens* tokens) {
  Token t = pop_token(tokens); // popping return type column
  t = pop_token(tokens);
  return map_type(t.keyword);
}

ArgumentsNode* parse_arguments_node(Tokens* tokens) {
  ArgumentsNode* arguments = malloc(sizeof(ArgumentsNode));
  memset(arguments, 0, sizeof(ArgumentsNode));

  pop_token(tokens); // popping the opening param
  Token t = peek_token(tokens);
  if (t.kind == TokenKind_Close_Paren) {
    pop_token(tokens); // popping the close paren
    return arguments;
  }

  do {
    fail_if(arguments->arguments_count >= AST_MAX_ARGUMENTS,
        "%s:%d:%d: Error: function has more arguments then the language can handle\n",
        t.pos.filepath, t.pos.line, t.pos.column);

    t = pop_token(tokens);
    strncpy(arguments->args[arguments->arguments_count].name, t.raw, TOKEN_RAW_CAPACITY);
    arguments->args[arguments->arguments_count].type = parse_type(tokens);

    arguments->arguments_count++;
    t = peek_token(tokens);
    if (t.kind == TokenKind_Comma) {
        t = pop_token(tokens);
    }
  } while(t.kind == TokenKind_Comma);

  pop_token(tokens); // popping the closing param
  return arguments;
}

BodyNode* parse_body_node(Tokens* tokens) {
  BodyNode* body = malloc(sizeof(BodyNode));
  memset(body, 0, sizeof(BodyNode));

  pop_token(tokens); // popping the body
  pop_token(tokens); // popping the body
  return body;
}

FunctionNode* parse_function_node(Tokens* tokens) {
  FunctionNode* function = malloc(sizeof(FunctionNode));

  Token t = pop_token(tokens); // Poping the 'func' keyword
  t = pop_token(tokens);
  strncpy(function->name, t.raw, TOKEN_RAW_CAPACITY);

  function->arguments = parse_arguments_node(tokens);
  function->return_type = parse_type(tokens);
  function->body = parse_body_node(tokens);

  return function;
}

RootNode* parse_root_node(Tokens* tokens) {
  RootNode* root = malloc(sizeof(RootNode));
  root->function_count = 0;

  assert(root->function_count < FUNCTION_DEF_CAPACITY && "Bail out, out of function capacity.");
  Token t = peek_token(tokens);
  while (t.kind == TokenKind_Keyword && t.keyword == TokenKeywordKind_FunctionDef) {
    root->functions[root->function_count++] = parse_function_node(tokens);
    t = peek_token(tokens);
  }

  return root;
}

AST* parse_ast(Tokens* tokens) {
  AST* ast = malloc(sizeof(AST));
  ast->root = parse_root_node(tokens);
  return ast;
}
