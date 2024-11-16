#include "core/ast.h"

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
  pop_token(tokens); // popping the arguments
  // @TODO(n) handle function args
  pop_token(tokens); // popping the arguments
  return arguments;
}

BodyNode* parse_body_node(Tokens* tokens) {
  BodyNode* body = malloc(sizeof(BodyNode));
  pop_token(tokens); // popping the body
  // @TODO(n) handle body
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
