#ifndef TOKER_H
#define TOKER_H

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdint.h>
#include "position.h"
#include "util.h"

typedef enum {
  TokenKind_Literal,
  TokenKind_Identifier,
  TokenKind_Keyword,
  TokenKind_Column,
  TokenKind_Comma,
  TokenKind_Open_Paren,
  TokenKind_Close_Paren,
  TokenKind_Open_Curl_Paren,
  TokenKind_Close_Curl_Paren,
  TokenKind_Eq,
  TokenKind_Semi,
  TokenKind_UNKNOWN
} TokenKind;

typedef enum {
  TokenKeywordKind_FunctionDef,
  TokenKeywordKind_i32Type,
  TokenKeywordKind_UNKNOWN
} TokenKeywordKind;

typedef struct {
  TokenKind kind;
  TokenKeywordKind keyword;
  Position pos;
  char raw[TOKEN_RAW_CAPACITY];
} Token;


#ifdef COMPILER_DEBUG
const char* token_kind_to_str(TokenKind kind);
const char* token_keyword_kind_to_str(TokenKeywordKind kind);
#endif

TokenKeywordKind keyword_kind(const char* raw);
Token make_token(TokenKind kind, TokenKeywordKind keyword_kind, Position pos, const char* raw);

typedef struct {
  Token* tokens;
  uint32_t count;
  uint32_t capacity;
  uint32_t iter;
  const char* source;
} Tokens;

#define TOKENS_CAPACITY 1024 * 10
void init_tokens(Tokens* tokens);
void free_tokens(Tokens* tokens);
void push_token(Tokens* tokens, Token token);
void reset_iter_tokens(Tokens* tokens);
Token pop_token(Tokens* tokens);
Token peek_token(Tokens* tokens);
Tokens tokenize(const char* filepath, const char* source);

#endif // TOKER_H
