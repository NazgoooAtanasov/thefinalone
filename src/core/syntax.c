#include "core/syntax.h"
#include "core/toker.h"

void parse_body(Tokens* tokens) {
  Token t = pop_token(tokens);
  fail_if(t.kind != TokenKind_Open_Curl_Paren, 
          "%s:%d:%d: Error: Expected '%s', but got '%s'\n", 
          t.pos.filepath, t.pos.line, t.pos.column, "{", t.raw);

  t = pop_token(tokens);
  fail_if(t.kind != TokenKind_Close_Curl_Paren, 
          "%s:%d:%d: Error: Expected '%s', but got '%s'\n", 
          t.pos.filepath, t.pos.line, t.pos.column, "}", t.raw);
}

void parse_func(Tokens* tokens) {
  Token t = pop_token(tokens);
  fail_if(t.kind != TokenKind_Keyword || t.keyword != TokenKeywordKind_FunctionDef, 
          "%s:%d:%d: Error: Expected '%s', but got '%s'\n", 
          t.pos.filepath, t.pos.line, t.pos.column, "func", t.raw);

  t = pop_token(tokens);
  fail_if(t.kind != TokenKind_Identifier, 
          "%s:%d:%d: Error: Expected '%s', but got '%s'\n", 
          t.pos.filepath, t.pos.line, t.pos.column, "identifier", t.raw);

  t = pop_token(tokens);
  fail_if(t.kind != TokenKind_Open_Paren, 
          "%s:%d:%d: Error: Expected '%s', but got '%s'\n", 
          t.pos.filepath, t.pos.line, t.pos.column, "(", t.raw);

  t = pop_token(tokens);
  fail_if(t.kind != TokenKind_Close_Paren, 
          "%s:%d:%d: Error: Expected '%s', but got '%s'\n", 
          t.pos.filepath, t.pos.line, t.pos.column, ")", t.raw);

  t = pop_token(tokens);
  fail_if(t.kind != TokenKind_Column, 
          "%s:%d:%d: Error: Expected '%s', but got '%s'\n", 
          t.pos.filepath, t.pos.line, t.pos.column, ":", t.raw);

  t = pop_token(tokens);
  fail_if(t.kind != TokenKind_Keyword || t.keyword != TokenKeywordKind_i32Type, // @TODO(n): in the future, make sure this handles all kinds of different types 
          "%s:%d:%d: Error: Expected '%s', but got '%s'\n", 
          t.pos.filepath, t.pos.line, t.pos.column, "i32", t.raw);

  parse_body(tokens);
}

void parse_root(Tokens* tokens) {
  Token t = peek_token(tokens);
  while (t.kind == TokenKind_Keyword && t.keyword == TokenKeywordKind_FunctionDef) {
    parse_func(tokens);
    t = peek_token(tokens);
  }
}

void parse_syntax(Tokens* tokens) {
  parse_root(tokens);
}
