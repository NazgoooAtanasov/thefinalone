#ifndef TARGET_H
#define TARGET_H

#include "core/ast.h"
#include "vendor/string_builder.h"

typedef enum {
  CompileTarget_x86_64_nasm,
  CompileTarget_UKNOWN_TARGET
} CompileTarget;

uint32_t compile_type_size(Type type);
void compile_to_target(CompileTarget target, AST* ast);

#endif // TARGET_H
