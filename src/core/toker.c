#include "core/toker.h"
#include "util.h"

#define is_whitespace(c) c == ' ' || c == '\t'
#define is_num(c) (c >= '0' && c <= '9')
#define is_alpha_num(c) (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c == '_') || is_num(c)

bool is_literal(const char* word) {
  size_t len = strlen(word);
  for (size_t i = 0; i < len; ++i) {
    if (!is_num(word[i])) return false;
  }
  return true;
}

bool is_intrinsic(const char* word) {
  if (strcmp(word, "INTR_EXIT") == 0) {
    return true;
  }

  return false;
}

#ifdef COMPILER_DEBUG
const char* token_kind_to_str(TokenKind kind) {
  switch(kind) {
    case TokenKind_Open_Curl_Paren: return "Open_Curl_Paren";
    case TokenKind_Close_Curl_Paren: return "Close_Curl_Paren";
    case TokenKind_Open_Paren: return "Open_Paren";
    case TokenKind_Close_Paren: return "Close_Paren";
    case TokenKind_Column: return "Column";
    case TokenKind_Literal: return "Literal";
    case TokenKind_Identifier: return "Identifier";
    case TokenKind_Keyword: return "Keyword";
    case TokenKind_Comma: return "Comma";
    case TokenKind_Intrinsic: return "Intrinsic";
    case TokenKind_Eq: return "Eq";
    case TokenKind_Semi: return "Semi";
    default: return "Unknown";
  }
}

const char* token_keyword_kind_to_str(TokenKeywordKind kind) {
  switch(kind) {
    case TokenKeywordKind_FunctionDef: return "FunctionDef";
    case TokenKeywordKind_i32Type: return "i32Type";
    default: return "Unknown";
  }
}
#endif

TokenKeywordKind keyword_kind(const char* raw) {
  if (strcmp(raw, "func") == 0) {
    return TokenKeywordKind_FunctionDef;
  }

  if (strcmp(raw, "i32") == 0) {
    return TokenKeywordKind_i32Type;
  }

  if (strcmp(raw, "void") == 0) {
    return TokenKeywordKind_voidType;
  }

  assert(TokenKeywordKind_UNKNOWN == 3 && "New keyword kind has been added but not handled!");
  return TokenKeywordKind_UNKNOWN;
}

Token make_token(TokenKind kind, TokenKeywordKind keyword_kind, Position pos, const char* raw) {
  Token token = {0};
  token.kind = kind;
  token.pos = pos;
  token.keyword = keyword_kind;
  strncpy(token.raw, raw, TOKEN_RAW_CAPACITY);
  return token;
}

void mark_point_tokens(Tokens* tokens) {
  tokens->_mark = tokens->iter;
}

void reset_point_tokens(Tokens* tokens) {
  tokens->iter = tokens->_mark;
  tokens->_mark = 0;
}

Token pop_token(Tokens* tokens) {
  assert(tokens->iter <= tokens->count && "Bail out, out of tokens range.");
  return tokens->tokens[tokens->iter++];
}

Token peek_token(Tokens* tokens) {
  assert(tokens->iter <= tokens->count && "Bail out, out of tokens range.");
  return tokens->tokens[tokens->iter];
}

void init_tokens(Tokens* tokens) {
  tokens->tokens = malloc(TOKENS_CAPACITY * sizeof(Token));
  tokens->count = 0;
  tokens->capacity = TOKENS_CAPACITY;
  tokens->_mark = 0;
}

void free_tokens(Tokens* tokens) {
  free(tokens->tokens);
}

void push_token(Tokens* tokens, Token token) {
  assert(tokens->count + 1 < tokens->capacity && "Bail out, out of tokens capacity.");
  tokens->tokens[tokens->count++] = token;
}

void reset_iter_tokens(Tokens* tokens) {
  tokens->iter = 0;
}

Tokens tokenize(const char* filepath, const char* source) {
  Tokens tokens = {0};
  tokens.source = filepath;
  init_tokens(&tokens);

  uint32_t i = 0;
  Position pos = {0};
  while (source[i] != '\0') {
    if (source[i] == '\n') {
      i++;
      move_down(pos);
      continue;
    }

    if (is_whitespace(source[i])) {
      i++;
      move_right(pos);
      continue;
    }

    char buff[TOKEN_RAW_CAPACITY] = {0};
    uint32_t buff_idx = 0;
    while (is_alpha_num(source[i])) {
      buff[buff_idx++] = source[i++];
      move_right(pos);
    }

    if (buff_idx > 0) {
      TokenKind kind = TokenKind_Identifier;
      TokenKeywordKind keyword = keyword_kind(buff);

      if (keyword != TokenKeywordKind_UNKNOWN) {
        kind = TokenKind_Keyword;
      }

      if (is_literal(buff)) {
        kind = TokenKind_Literal;
      }

      if (is_intrinsic(buff)) {
        kind = TokenKind_Intrinsic;
      }

      Token token = make_token(kind, keyword, (Position) {.filepath = tokens.source, .column = pos.column - buff_idx, .line = pos.line}, buff);
      push_token(&tokens, token);
      continue;
    }

    TokenKind kind = TokenKind_UNKNOWN;
    switch(source[i]) {
      case '{': kind = TokenKind_Open_Curl_Paren; break;
      case '}': kind = TokenKind_Close_Curl_Paren; break;
      case '(': kind = TokenKind_Open_Paren; break;
      case ')': kind = TokenKind_Close_Paren; break;
      case ':': kind = TokenKind_Column; break;
      case ';': kind = TokenKind_Semi; break;
      case ',': kind = TokenKind_Comma; break;
      case '=': kind = TokenKind_Eq; break;
      default: fail_if(true, "Unknown token found at line %d, column %d\n", pos.line, pos.column - buff_idx);
    }
    fail_if(kind == TokenKind_UNKNOWN, "Unknown token found at line %d, column %d\n", pos.line, pos.column - buff_idx);

    buff[0] = source[i];
    Token token = make_token(kind, TokenKeywordKind_UNKNOWN, (Position) {.filepath = tokens.source, .column = pos.column - buff_idx, .line = pos.line}, buff);
    push_token(&tokens, token);
    i++;
    move_right(pos);
  }

  return tokens;
}
