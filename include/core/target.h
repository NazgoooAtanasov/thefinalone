#ifndef TARGET_H
#define TARGET_H

#include "core/ast.h"
#include "vendor/string_builder.h"

typedef enum {
  IntrinsicKind_UNKNOWN,
  IntrinsicKind_EXIT
} IntrinsicKind;
IntrinsicKind map_intrinsic_kind(const char* word);

typedef enum {
  CompileTarget_x86_64_nasm,
  CompileTarget_UKNOWN_TARGET
} CompileTarget;

// definition is located in target.c
extern const char* register_map[]; 

uint32_t compile_type_size(Type type);
void compile_to_target(CompileTarget target, AST* ast);

#endif // TARGET_H
