#include "core/ast.h"
#include "core/toker.h"
#include "util.h"

#ifdef COMPILER_DEBUG
const char* ast_type_to_str(Type type) {
  switch(type) {
    case Type_i32: return "i32";
    case Type_void: return "void";
    default: fail_if(true, "Bail out, unknown type.");
  }
}

const char* ast_statement_type_to_str(StatementType type) {
  switch(type) {
    case StatementType_VariableAssign: return "VariableAssign";
    case StatementType_FunctionCall: return "FunctionCall";
    default: fail_if(true, "Bail out, unknown statement type.");
  }
}
#endif

Type map_type(TokenKeywordKind kind) {
  switch(kind) {
    case TokenKeywordKind_i32Type: return Type_i32;
    case TokenKeywordKind_voidType: return Type_void;
    default: return Type_UNKNOWN;
  }
}

Type parse_type(Tokens* tokens) {
  Token t = pop_token(tokens); // popping return type column
  t = pop_token(tokens);
  return map_type(t.keyword);
}

ArgumentsNode* parse_arguments_node(Tokens* tokens) {
  ArgumentsNode* arguments = malloc(sizeof(ArgumentsNode));
  memset(arguments, 0, sizeof(ArgumentsNode));

  Token t = pop_token(tokens); // popping the opening param
  t = peek_token(tokens);
  arguments->_meta.start = t.pos;

  if (t.kind == TokenKind_Close_Paren) {
    t = pop_token(tokens); // popping the close paren
    arguments->_meta.end = t.pos;
    return arguments;
  }

  do {
    fail_if(arguments->arguments_count >= AST_MAX_ARGUMENTS,
            "%s:%d:%d: Error: function has more arguments then the language can handle\n",
            t.pos.filepath, t.pos.line, t.pos.column);

    t = pop_token(tokens);
    strncpy(arguments->args[arguments->arguments_count].name, t.raw, TOKEN_RAW_CAPACITY);
    arguments->args[arguments->arguments_count].type = parse_type(tokens);
    fail_if(arguments->args[arguments->arguments_count].type == Type_UNKNOWN, "%s:%d:%d: Error: argument type annotation expected but got something else\n",
            t.pos.filepath,
            t.pos.line,
            t.pos.column);

    arguments->arguments_count++;
    t = peek_token(tokens);
    if (t.kind == TokenKind_Comma) {
      t = pop_token(tokens);
    }
  } while(t.kind == TokenKind_Comma);

  t = pop_token(tokens); // popping the closing param
  arguments->_meta.end = t.pos;
  return arguments;
}

Result parse_variable_assign_node(Tokens* tokens) {
  Result result = result_create_empty();

  Token name = pop_token(tokens);
  if (name.kind != TokenKind_Identifier) {
    result_set_error(result,
                     "%s:%d:%d: Error: Expected identifier, found '%s'",
                     name.pos.filepath, name.pos.line, name.pos.column, name.raw);
    return result;
  }

  Type type = parse_type(tokens);
  if (type == Type_UNKNOWN) {
    result_set_error(result,
                     "%s:%d:%d: Error: Expected type annotation, found '%s'",
                     name.pos.filepath, name.pos.line, name.pos.column, name.raw);
    return result;
  } 

  Token t = pop_token(tokens);
  if (t.kind != TokenKind_Eq) {
    result_set_error(result,
                     "%s:%d:%d: Error: Expected '%c', found '%s'",
                     name.pos.filepath, name.pos.line, name.pos.column, '=', name.raw);
    return result;
  }

  Token value = pop_token(tokens);
  if (value.kind != TokenKind_Literal) {
    result_set_error(result,
                     "%s:%d:%d: Error: Expected literal value, found '%s'",
                     name.pos.filepath, name.pos.line, name.pos.column, name.raw);
    return result;
  }

  t = pop_token(tokens);
  if (t.kind != TokenKind_Semi) {
    result_set_error(result,
                     "%s:%d:%d: Error: Expected '%c', found '%s'",
                     name.pos.filepath, name.pos.line, name.pos.column, ';', name.raw);
    return result;
  }

  VariableAssignStatement* variable_assign = malloc(sizeof(VariableAssignStatement));

  strncpy(variable_assign->name, name.raw, TOKEN_RAW_CAPACITY);
  variable_assign->type = type;
  strncpy(variable_assign->literal_value, value.raw, TOKEN_RAW_CAPACITY);
  variable_assign->_meta.start = name.pos;
  variable_assign->_meta.end = t.pos;

  result.value = variable_assign;

  return result;
}

Result parse_function_call_node(Tokens* tokens) {
  (void) tokens;
  Result result = result_create_empty();

  Token function_name = pop_token(tokens);
  if (function_name.kind != TokenKind_Identifier && function_name.kind != TokenKind_Intrinsic) {
    result_set_error(result, "%s:%d:%d: Error: Expected identifier or intrinsic for function call, but got found '%s'", 
                     function_name.pos.filepath, function_name.pos.line, function_name.pos.column, 
                     function_name.raw
                     );
    return result;
  }

  Token t = pop_token(tokens);
  if (t.kind != TokenKind_Open_Paren) {
    result_set_error(result, "%s:%d:%d: Error: Expected '(' for function call, but got found '%s'", 
                     t.pos.filepath, t.pos.line, t.pos.column, 
                     t.raw
                     );
    return result;
  }

  t = peek_token(tokens);
  while (t.kind != TokenKind_Close_Paren) {
    t = pop_token(tokens);
    if (t.kind != TokenKind_Identifier && t.kind != TokenKind_Literal) {
      result_set_error(result, "%s:%d:%d: Error: Expected identifier or literal for function call argument, but got found '%s'", 
                       t.pos.filepath, t.pos.line, t.pos.column, 
                       t.raw
                       );
      return result;
    }

    t = peek_token(tokens);
  }

  t = pop_token(tokens);
  if (t.kind != TokenKind_Close_Paren) {
    result_set_error(result, "%s:%d:%d: Error: Expected ')' for function call, but got found '%s'", 
                     t.pos.filepath, t.pos.line, t.pos.column, 
                     t.raw
                     );
    return result;
  }

  t = pop_token(tokens);
  if (t.kind != TokenKind_Semi) {
    result_set_error(result, "%s:%d:%d: Error: Expected ';' for function call, but got found '%s'", 
                     t.pos.filepath, t.pos.line, t.pos.column, 
                     t.raw
                     );
    return result;
  }

  FunctionCallStatement* function_call = malloc(sizeof(FunctionCallStatement));
  strncpy(function_call->name, function_name.raw, TOKEN_RAW_CAPACITY);
  function_call->_meta.start = function_name.pos;
  function_call->_meta.end = t.pos;
  function_call->is_intrinsic = function_name.kind == TokenKind_Intrinsic;

  result.value = function_call;

  return result;
}

BodyNode* parse_body_node(Tokens* tokens) {
  BodyNode* body = malloc(sizeof(BodyNode));
  memset(body, 0, sizeof(BodyNode));
  body->variables = assoc_array_new();

  Token t = pop_token(tokens); // popping the body
  body->_meta.start = t.pos;

  t = peek_token(tokens);
  while (t.kind != TokenKind_Close_Curl_Paren) {
    if (t.kind == TokenKind_Identifier) {
      mark_point_tokens(tokens);
        Result variable_assign = parse_variable_assign_node(tokens);
        if (result_is_ok(&variable_assign)) {
          VariableAssignStatement* var = result_unwrap(&variable_assign);
          body->statements[body->statements_count].type = StatementType_VariableAssign;
          body->statements[body->statements_count].statement = var;
          body->statements[body->statements_count]._meta.start = var->_meta.start;
          body->statements[body->statements_count]._meta.end = var->_meta.end;
          body->statements_count++;
          t = peek_token(tokens);
          continue;
        }
      reset_point_tokens(tokens);

      mark_point_tokens(tokens);
        Result function_call = parse_function_call_node(tokens);
        if (result_is_ok(&function_call)) {
          FunctionCallStatement* call = result_unwrap(&function_call);
          body->statements[body->statements_count].type = StatementType_FunctionCall;
          body->statements[body->statements_count].statement = call;
          body->statements[body->statements_count]._meta.start = call->_meta.start;
          body->statements[body->statements_count]._meta.end = call->_meta.end;
          body->statements_count++;
          t = peek_token(tokens);
          continue;
        }
      reset_point_tokens(tokens);
    }

    if (t.kind == TokenKind_Intrinsic) {
      mark_point_tokens(tokens);
        Result function_call = parse_function_call_node(tokens);
        if (result_is_ok(&function_call)) {
          FunctionCallStatement* call = result_unwrap(&function_call);
          body->statements[body->statements_count].type = StatementType_FunctionCall;
          body->statements[body->statements_count].statement = call;
          body->statements[body->statements_count]._meta.start = call->_meta.start;
          body->statements[body->statements_count]._meta.end = call->_meta.end;
          body->statements_count++;
          t = peek_token(tokens);
          continue;
        }
      reset_point_tokens(tokens);
    }

    t = peek_token(tokens);
  }

  t = pop_token(tokens); // popping the body
  body->_meta.end = t.pos;
  return body;
}

FunctionNode* parse_function_node(Tokens* tokens) {
  FunctionNode* function = malloc(sizeof(FunctionNode));

  Token t = pop_token(tokens); // Poping the 'func' keyword
  function->_meta.start = t.pos;

  t = pop_token(tokens);
  strncpy(function->name, t.raw, TOKEN_RAW_CAPACITY);

  function->arguments = parse_arguments_node(tokens);
  function->return_type = parse_type(tokens);
  fail_if(function->return_type == Type_UNKNOWN, "%s:%d:%d: Error: Expected type annotation for return type of the function but got something else.\n", 
          function->_meta.start.filepath, function->_meta.start.line, function->_meta.start.column);
  function->body = parse_body_node(tokens);
  function->_meta.end = function->body->_meta.end;

  return function;
}

RootNode* parse_root_node(Tokens* tokens) {
  RootNode* root = malloc(sizeof(RootNode));
  root->function_count = 0;

  assert(root->function_count < FUNCTION_DEF_CAPACITY && "Bail out, out of function capacity.");
  Token t = peek_token(tokens);
  root->_meta.start = t.pos;

  while (t.kind == TokenKind_Keyword && t.keyword == TokenKeywordKind_FunctionDef) {
    root->functions[root->function_count++] = parse_function_node(tokens);
    t = peek_token(tokens);
  }

  root->_meta.end = t.pos;
  return root;
}

AST* parse_ast(Tokens* tokens) {
  AST* ast = malloc(sizeof(AST));
  ast->root = parse_root_node(tokens);
  return ast;
}
