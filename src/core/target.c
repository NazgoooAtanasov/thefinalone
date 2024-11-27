#include "core/target.h"
#include "vendor/string_builder.h"
#include <stdio.h>

const char* register_map[] = {
  "rdi", "rsi", "rdx", "r10", "r8", "r9",
};

uint32_t compile_type_size(Type type) {
  switch (type) {
    case Type_i32: return 4; // 4 bytes 
    case Type_i64: return 8; // 8 bytes 
    default: fail_if(true, "UNREACHABLE");
  }
}

const char* compile_type_word(Type type) {
  switch (type) {
    case Type_i32: return "dword"; // 4 bytes 
    case Type_i64: return "qword"; // 8 bytes 
    default: fail_if(true, "UNREACHABLE");
  }
}

IntrinsicKind map_intrinsic_kind(const char* word) {
  if (strcmp(word, "INTR_EXIT") == 0)  return IntrinsicKind_EXIT;
  return IntrinsicKind_UNKNOWN;
}

void compile_body_node(BodyNode* body, StringBuilder* sb) {
  uint32_t local_variables_size = 0;

  for (uint32_t i = 0; i < body->statements_count; ++i) {
    StatementNode statement = body->statements[i];
    if (statement.type == StatementType_VariableAssign) {
      VariableAssignStatement* assign = statement.statement;
      local_variables_size += compile_type_size(assign->type);
    }
  }

  if (local_variables_size > 0) {
    string_builder_append_strf(sb, "\tsub rsp, %d\n", local_variables_size);
  };

  uint32_t offset = 0;
  for (uint32_t i = 0; i < body->statements_count; ++i) {
    StatementNode statement = body->statements[i];
    if (statement.type == StatementType_VariableAssign) {
      VariableAssignStatement* assign = statement.statement;

      uint32_t type_size = compile_type_size(assign->type);
      offset += type_size;
      string_builder_append_strf(sb, "\tmov %s [rpb-%d], %s\t; variable name %s\n",
                                 compile_type_word(assign->type),
                                 offset,
                                 assign->literal_value,
                                 assign->name);
    } 

    if (statement.type == StatementType_FunctionCall) {
      FunctionCallStatement* call = statement.statement;
      if (call->is_intrinsic) {
        IntrinsicKind intrinsic_kind = map_intrinsic_kind(call->name);
        switch (intrinsic_kind) {
          case IntrinsicKind_EXIT: 
            string_builder_append_strf(sb, "\tmov rax, 60\t; INTR_EXIT\n");

            for (uint32_t i = 0; i < call->arguments_count; ++i) {
              const char* arg = call->arguments[i];
              string_builder_append_strf(sb, "\tmov %s, %s\n", register_map[i], arg);
            }

            string_builder_append_strf(sb, "\tsyscall\n");
            break;
          default: 
            fail_if(true, "UNREACHABLE");
        }
      } else {
        string_builder_append_strf(sb, "\tcall %s\t; function call to %s\n", call->name, call->name);
      }
    }
  }
}

void compile_function_node(FunctionNode* function, StringBuilder* sb) {
  string_builder_append_str(sb, function->name, strlen(function->name));
  string_builder_append_str(sb, ":\n", 2);

  const char* stack_frame_begin = "\tpush rbp\n"
    "\tmov rbp, rsp\n";

  string_builder_append_str(sb, stack_frame_begin, strlen(stack_frame_begin));

  compile_body_node(function->body, sb);

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
