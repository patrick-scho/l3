#include "vm.h"


Value *statement_run(Statement *stmt, Context *ctx) {
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
        return context_run(ctx->statements[index+1]->param2);
    }
  case STMT_ELSE:
  case STMT_STRUCT_MEMBER_SET:
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

