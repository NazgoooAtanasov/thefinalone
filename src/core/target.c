#include "core/target.h"
#include "vendor/string_builder.h"
#include <stdio.h>

void compile_function_node(FunctionNode* function, StringBuilder* sb) {
  string_builder_append_str(sb, function->name, strlen(function->name));
  string_builder_append_str(sb, ":\n", 2);

  const char* stack_frame_begin = "\tpush rbp\n"
    "\tmov rbp, rsp\n";

  string_builder_append_str(sb, stack_frame_begin, strlen(stack_frame_begin));

  // @TODO(n): compile body

  const char* stack_frame_end = "\tmov rps, rbp\n"
    "\tpop rbp\n"
    "\tret\n";
  string_builder_append_str(sb, stack_frame_end, strlen(stack_frame_end));
}

void compile_to_x86_64_nasm(AST* ast, StringBuilder* sb) {
  for (uint32_t i = 0; i < ast->root->function_count; ++i) {
    compile_function_node(ast->root->functions[i], sb);
  }
}

void compile_to_target(CompileTarget target, AST* ast) {
  assert(ast != NULL && "Bail out. AST cannot be null at that point");

  StringBuilder sb = {0};

  const char* begin = "BITS 64\n"
    "global _start\n"
    "\n";
  string_builder_append_str(&sb, begin, strlen(begin));

  switch (target) {
    case CompileTarget_x86_64_nasm: compile_to_x86_64_nasm(ast, &sb); break;
    case CompileTarget_UKNOWN_TARGET: fail_if(true, "Unknown compile target found"); break;
  }

  const char* end = "\n"
    "_start:\n"
    "\tcall main\n";
  string_builder_append_str(&sb, end, strlen(end));

  printf("%s\n", sb.string);
}
