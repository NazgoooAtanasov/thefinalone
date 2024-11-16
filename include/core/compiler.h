#ifndef COMPILER_H
#define COMPILER_H

#include <errno.h>
#include <string.h>
#include <assert.h>
#include "core/ast.h"

AST* compile(const char* filepath);

#endif // COMPILER_H

