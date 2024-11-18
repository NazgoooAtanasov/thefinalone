#define STRING_BUILDER_IMPLEMENTATION
#include "vendor/string_builder.h"

#define RESULT_H_IMPLEMENTATION
#include "vendor/result.h"

#include "core/cmd.h"

int main(int argc, char** argv) {
  Cmd cmd = parse_cmd(argc, argv);
  execute_cmd(&cmd);
  return 0;
}
