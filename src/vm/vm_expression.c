#include "vm.h"


Value *expression_run(Expression *expr, Context *ctx) {
  switch (expr->type) {
  case EXPR_BINARY:
    return value_binary(expr->binary.op,
      expression_run(expr->binary.expr1, ctx),
      expression_run(expr->binary.expr2, ctx));
  case EXPR_UNARY:
    return value_unary(expr->unary.op,
      expression_run(expr->unary.expr, ctx));
  case EXPR_LITERAL:
    return expr->literal.value;
  case EXPR_VAR_GET: {
    char *name = expr->var_get.name;
    Variable *v = context_variable_get(ctx, name);
    if (v != NULL)
      return v->value;
    break;
  case EXPR_STRUCT_MEMBER_GET:
    break;
  }
  }
  return NULL;
}

