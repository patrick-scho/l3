#include "vm.h"

Statement *statement_create(StatementType type, void *param1, void *param2) {
  Statement *result = malloc(sizeof(Statement));

  result->type = type;
  result->param1 = param1;
  result->param2 = param2;
  
  return result;
}


Value statement_run(Statement *stmt, Context *ctx) {
  switch (stmt->type) {
  case STMT_RETURN:
    return expression_run(stmt->param1, ctx);
  case STMT_EXPR:
    expression_run(stmt->param1, ctx);
    break;
  case STMT_IF:
    if (expression_run(stmt->param1, ctx))
      return context_run(stmt->param2);
    else {
      int index = context_get_statement_index(ctx, stmt);
      if (
        index >= 0 &&
        index < arrlen(ctx->statements)-1 &&
        ctx->statements[index+1]->type == STMT_ELSE)
        context_run(ctx->statements[index+1]->param2);
    }
    break;
  case STMT_WHILE:
    while (expression_run(stmt->param1, ctx)) {
      void *result = context_run(stmt->param2);
      if (result != NULL)
        return result;
    }
    break;
  case STMT_CTX:
    return context_run(stmt->param2);
    break;
  case STMT_VAR_SET: {
    Variable *v = stmt->param1;
    Expression *value = stmt->param2;
    if (v != NULL)
      v->value = expression_run(value, ctx);
    break;
  }
  }
  return NULL;
}


void statement_destroy(Statement *stmt) {
  switch (stmt->type) {
  case STMT_EXPR:
  case STMT_RETURN:
    expression_destroy(stmt->param1);
    break;
  case STMT_IF:
  case STMT_WHILE:
    expression_destroy(stmt->param1);
    context_destroy(stmt->param2);
    break;
  case STMT_CTX:
  case STMT_ELSE:
    context_destroy(stmt->param2);
    break;
  case STMT_VAR_SET:
    free((char*)stmt->param1);
    expression_destroy(stmt->param2);
    break;
  }

  free(stmt);
}
