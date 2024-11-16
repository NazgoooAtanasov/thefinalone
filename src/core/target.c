#include "core/target.h"

void compile_to_x86_64_nasm(AST* ast, StringBuilder* sb) {
  for (uint32_t i = 0; i < ast->root->function_count; ++i) {
    FunctionNode* function = ast->root->functions[i];

    string_builder_append_str(sb, function->name, strlen(ast->root->functions[i]->name));
    string_builder_append_chr(sb, ':');
  }
}

void compile_to_target(CompileTarget target, AST* ast) {
  assert(ast != NULL && "Bail out. AST cannot be null at that point");

  StringBuilder sb = {0};

  switch (target) {
    case CompileTarget_x86_64_nasm: return compile_to_x86_64_nasm(ast, &sb);
    case CompileTarget_UKNOWN_TARGET: fail_if(true, "Unknown compile target found"); break;
  }
}
