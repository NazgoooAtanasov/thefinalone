#ifndef CMD_H
#define CMD_H

#include <stdbool.h>
#include <assert.h>

typedef enum {
  CmdKind_Compile,
  CmdKind_UNKNOWN
} CmdKind;

#define Cmd_ArgumentCount 256
typedef struct {
  CmdKind kind;
  const char* arguments[Cmd_ArgumentCount];
} Cmd;

Cmd parse_cmd(int argc, char** argv);
void execute_cmd(Cmd* cmd);

#endif // CMD_H
