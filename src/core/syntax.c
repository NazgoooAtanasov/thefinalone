#include "core/syntax.h"
#include "core/toker.h"
#include "util.h"
#include "vendor/result.h"

bool keyword_is_type(TokenKeywordKind keyword_kind) {
  switch (keyword_kind) {
    case TokenKeywordKind_voidType:
    case TokenKeywordKind_i32Type: 
      return true;

    case TokenKeywordKind_FunctionDef: return false;
    default: fail_if(true, "Bail out. UNKNOWN keyword kind")
  }
}

void parse_func_args(Tokens* tokens) {
  Token t = peek_token(tokens);

  if (t.kind == TokenKind_Identifier) {
    do {
      t = pop_token(tokens); // poping the identifier

      t = pop_token(tokens);
      fail_if(t.kind != TokenKind_Column,
              "%s:%d:%d: Error: Expected '%s', but got '%s'\n",
              t.pos.filepath, t.pos.line, t.pos.column, ":", t.raw);

      t = pop_token(tokens);
      fail_if(t.kind != TokenKind_Keyword || !keyword_is_type(t.keyword),
              "%s:%d:%d: Error: Expected type, but got '%s'\n",
              t.pos.filepath, t.pos.line, t.pos.column, t.raw);

      t = peek_token(tokens);
      if (t.kind == TokenKind_Comma) {
        t = pop_token(tokens);
      }
    } while(t.kind == TokenKind_Comma);

    return;
  } else if (t.kind == TokenKind_Close_Paren) {
    return;
  }

  fail_if(true, "%s:%d:%d: Error: Expected either a function argument parameter or ')', but got '%s'\n",
          t.pos.filepath, t.pos.line, t.pos.column, t.raw);
}

Result parse_variable_assign(Tokens* tokens) {
  Result result = result_create_empty();
  Token t = pop_token(tokens); // variable name

  t = pop_token(tokens);
  if (t.kind != TokenKind_Column) {
    result_set_error(result, "%s:%d:%d: Error: Expected '%s', but got '%s'", 
                     t.pos.filepath, t.pos.line, t.pos.column,
                     ":", t.raw);
    return result;
  }

  t = pop_token(tokens);
  if (t.kind != TokenKind_Keyword || t.keyword != TokenKeywordKind_i32Type) {
    result_set_error(result, "%s:%d:%d: Error: Expected a type, but got '%s'", 
                     t.pos.filepath, t.pos.line, t.pos.column, t.raw);
    return result;
  }

  t = pop_token(tokens);
  if (t.kind != TokenKind_Eq) {
    result_set_error(result, "%s:%d:%d: Error: Expected '%c', but got '%s'", 
                     t.pos.filepath, t.pos.line, t.pos.column, '=', t.raw);
    return result;
  }

  t = pop_token(tokens);
  if (t.kind != TokenKind_Literal) {
    result_set_error(result, "%s:%d:%d: Error: Expected literal value, but got '%s'", 
                     t.pos.filepath, t.pos.line, t.pos.column, t.raw);
    return result;
  }

  t = pop_token(tokens);
  if (t.kind != TokenKind_Semi) {
    result_set_error(result, "%s:%d:%d: Error: Expected '%c', but got '%s'", 
                     t.pos.filepath, t.pos.line, t.pos.column, ';', t.raw);
    return result;
  }

  return result;
}

void parse_statement(Tokens* tokens) {
  Token t = peek_token(tokens);
  if (t.kind == TokenKind_Identifier) {
    Result variable = parse_variable_assign(tokens);

    if (result_is_ok(&variable)) {
      return;
    }

    fail_if(result_is_err(&variable), "%s\n", result_error(&variable));
  }
}

void parse_body(Tokens* tokens) {
  Token t = pop_token(tokens);
  fail_if(t.kind != TokenKind_Open_Curl_Paren,
          "%s:%d:%d: Error: Expected '%s', but got '%s'\n",
          t.pos.filepath, t.pos.line, t.pos.column, "{", t.raw);

  t = peek_token(tokens);
  while (t.kind != TokenKind_Close_Curl_Paren) {
    parse_statement(tokens);
    t = peek_token(tokens);
  }

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

  parse_func_args(tokens);

  t = pop_token(tokens);
  fail_if(t.kind != TokenKind_Close_Paren,
          "%s:%d:%d: Error: Expected '%s', but got '%s'\n",
          t.pos.filepath, t.pos.line, t.pos.column, ")", t.raw);

  t = pop_token(tokens);
  fail_if(t.kind != TokenKind_Column,
          "%s:%d:%d: Error: Expected '%s', but got '%s'\n",
          t.pos.filepath, t.pos.line, t.pos.column, ":", t.raw);

  t = pop_token(tokens);
  fail_if(t.kind != TokenKind_Keyword || !keyword_is_type(t.keyword),
          "%s:%d:%d: Error: Expected type, but got '%s'\n",
          t.pos.filepath, t.pos.line, t.pos.column, t.raw);

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
