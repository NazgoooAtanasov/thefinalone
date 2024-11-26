#include "core/semantic.h"

void check_function_arguments(FunctionNode* function) {
  fail_if(function->arguments->arguments_count > AST_MAX_ARGUMENTS,
          "%s:%d:%d: Error: function with name %s has more arguments then the language can handle",
          function->_meta.start.filepath,
          function->_meta.start.line,
          function->_meta.start.column,
          function->name);
}

void check_body(BodyNode* body) {
  // @TODO(n): handle function calls
  for (uint32_t i = 0; i < body->statements_count; ++i) {
    StatementNode statement = body->statements[i];
    if (statement.type == StatementType_VariableAssign) {
      VariableAssignStatement* assign = statement.statement;

      fail_if(assoc_array_has(&body->variables, assign->name), 
              "%s:%d:%d: Error: Variable with the name '%s' already exists\n",
              statement._meta.start.filepath,
              statement._meta.start.line,
              statement._meta.start.column,
              assign->name
              );

      assoc_array_put(&body->variables, assign->name, assign->literal_value);
    } 
  }
}

void check_function(FunctionNode* function) {
  check_function_arguments(function);
  check_body(function->body);
}

void check_root(RootNode* root) {
  bool has_main = false;
  for (uint32_t i = 0; i < root->function_count; ++i) {
    check_function(root->functions[i]);
    if (!has_main && strcmp(root->functions[i]->name, "main") == 0) {
      has_main = true;
    }
  }

  fail_if(!has_main, "%s:%d:%d: Error: No main function found\n", root->_meta.start.filepath, root->_meta.start.line, root->_meta.start.column);
}

void semantic_parse(AST* ast) {
  check_root(ast->root);
}
