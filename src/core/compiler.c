#include "util.h"
#include "core/toker.h"
#include "core/syntax.h"
#include "core/compiler.h"
#include "core/semantic.h"

const char* file_contents(const char* filepath) {
  FILE* f = fopen(filepath, "r");
  fail_if(f == NULL, "Compiler failed: Cannot open file \"%s\"\n"
                     "\tError: %s\n", filepath, strerror(errno));

  fail_if(fseek(f, 0, SEEK_END) == -1, "Compiler failed: Cannot open file \"%s\"\n"
                                      "\tError: %s\n", filepath, strerror(errno));
  long fsize = ftell(f);
  fail_if(fsize == -1, "Compiler failed: Cannot open file \"%s\"\n"
                       "\tError: %s\n", filepath, strerror(errno));

  fail_if(fseek(f, 0, SEEK_SET) == -1, "Compiler failed: Cannot open file \"%s\"\n"
                                       "\tError: %s\n", filepath, strerror(errno));

  char* contents = malloc(fsize + 1);
  assert(contents != NULL && "Bail out, out of memory.");

  fail_if(fread(contents, sizeof(char), fsize, f) <= 0, "Compiler failed: Cannot read file \"%s\"\n"
                                                        "\tError: %s\n", filepath, strerror(errno));

  return contents;
}

AST* compile(const char* filepath) {
  const char* contents = file_contents(filepath);

  Tokens tokens = tokenize(filepath, contents);
  fail_if(tokens.count <= 0, "Compiler failed: No contents found in \"%s\"\n", filepath);
#ifdef COMPILER_DEBUG
  /* for (uint32_t i = 0; i < tokens.count; i++) { */
  /*   Token token = tokens.tokens[i]; */
  /*   printf("[Token: \"%s\", Kind: \"%s\", KeywordKind: \"%s\", Pos: %d:%d]\n", */
  /*          token.raw, */
  /*          token_kind_to_str(token.kind), */
  /*          token_keyword_kind_to_str(token.keyword), */
  /*          token.pos.line, */
  /*          token.pos.column */
  /*          ); */
  /* } */
#endif

  parse_syntax(&tokens);
  reset_iter_tokens(&tokens);
  AST* ast = parse_ast(&tokens);
  semantic_parse(ast);

  free_tokens(&tokens);
  free((void*)contents);

  return ast;
}
