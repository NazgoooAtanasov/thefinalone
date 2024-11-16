#include "core/semantic.h"
#include "util.h"

void check_function(FunctionNode* function) {
  (void) function;
  // @TODO(n): handle semantic checks per function
}

void check_root(RootNode* root) {
  bool has_main = false;
  for (uint32_t i = 0; i < root->function_count; ++i) {
    check_function(root->functions[i]);
    if (!has_main && strcmp(root->functions[i]->name, "main") == 0) {
      has_main = true;
    }
  }

  fail_if(!has_main, "filename:%d:%d: Error: No main function found\n", 0, 0);
}

void semantic_parse(AST* ast) {
  check_root(ast->root);
}
