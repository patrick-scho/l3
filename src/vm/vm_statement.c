#include "vm.h"

#include "vm_types.h"
#include <util/error.h>

Value *statement_run(Statement *stmt, Context *ctx) {
  switch (stmt->type) {
  case STMT_RETURN:
    return expression_run(stmt->expr.expr, ctx);
  case STMT_EXPR:
    expression_run(stmt->expr.expr, ctx);
    break;
  case STMT_IF: {
    Value *value = expression_run(stmt->expr_ctx.expr, ctx);
    if (!type_match(&type_bool, value->type))
      error("expected bool in IF");
    if ((bool)value->value)
      return context_run(stmt->expr_ctx.ctx);
    else {
      int index = context_get_statement_index(ctx, stmt);
      if (
        index >= 0 &&
        index < arrlen(ctx->statements)-1 &&
        ctx->statements[index+1]->type == STMT_ELSE)
        return context_run(ctx->statements[index+1]->ctx.ctx);
    }
    break;
  }
  case STMT_ELSE:
  case STMT_STRUCT_MEMBER_SET:
    break;
  case STMT_WHILE:
    while (true) {
      Value *value = expression_run(stmt->expr_ctx.expr, ctx);
      if (!type_match(&type_bool, value->type))
        error("expected bool in WHILE");
      if (! (bool)value->value)
        break;
      void *result = context_run(stmt->expr_ctx.ctx);
      if (result != NULL)
        return result;
    }
    break;
  case STMT_CTX:
    return context_run(stmt->expr_ctx.ctx);
    break;
  case STMT_VAR_SET: {
    Variable *v = context_variable_get(ctx, stmt->var_set.name);
    Expression *value = stmt->var_set.expr;
    if (v != NULL)
      v->value = expression_run(value, ctx);
    break;
  }
  }
  return NULL;
}

