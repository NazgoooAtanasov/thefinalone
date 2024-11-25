#ifndef SEMANTIC_H
#define SEMANTIC_H

#include <stdbool.h>
#include "core/ast.h"
#include "util.h"
#include "vendor/assoc_array.h"

void semantic_parse(AST* ast);

#endif // SEMANTIC_H
