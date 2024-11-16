#ifndef POSITION_H
#define POSITION_H

#include <stdint.h>

typedef struct {
  uint32_t line;
  uint32_t column;
  const char* filepath;
} Position;

#define move_right(pos) pos.column++
#define move_down(pos) pos.column = 0; pos.line++

#endif // POSITION_H
