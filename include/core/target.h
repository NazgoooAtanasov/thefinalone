#ifndef TARGET_H
#define TARGET_H

#include <assert.h>
#include "core/ast.h"
#include "vendor/string_builder.h"

typedef enum {
  CompileTarget_x86_64_nasm,
  CompileTarget_UKNOWN_TARGET
} CompileTarget;

void compile_to_target(CompileTarget target, AST* ast);

#endif // TARGET_H
