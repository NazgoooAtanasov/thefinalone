#include "core/cmd.h"
#include "core/ast.h"
#include "core/compiler.h"
#include "core/target.h"
#include "util.h"

Cmd parse_cmd(int argc, char** argv) {
  fail_if(argc < 2, 
          "Unexpected number of arguments\n"
          "\tExptected usage: thefinalone <cmd>\n");
  Cmd cmd;
  memset(&cmd, 0, sizeof(Cmd));

  const char* cmd_name = argv[1];

  if (str_eq(cmd_name, "compile")) {
    fail_if(argc < 3, 
            "Unexpected number of arguments\n"
            "\tExptected usage: thefinalone compile <file>\n");
    cmd.kind = CmdKind_Compile;
    cmd.arguments[0] = argv[2];
  } else {
    fail_if(true, "Unknown command\n");
  }

  assert(CmdKind_UNKNOWN == 1 && "There has been a new cmd kind added, handle its parsing!");
  return cmd;
}

void execute_cmd(Cmd* cmd) {
  switch (cmd->kind) {
    case CmdKind_Compile: {
      AST* ast = compile(cmd->arguments[0]);
      compile_to_target(CompileTarget_x86_64_nasm, ast);
      break;
    }

    case CmdKind_UNKNOWN:
    default: {
      fail_if(true, "Unknown command\n")
    }
  }

  assert(CmdKind_UNKNOWN == 1 && "There has been a new cmd kind added, handle its execution!");
}
