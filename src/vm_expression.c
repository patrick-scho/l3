#include "vm.h"

Expression *expression_create(ExpressionType type, void *params) {
  Expression *result = malloc(sizeof *result);
  result->type = type;
  result->params = params;
  return result;
}


void *expression_run(Context *ctx, Expression *expr) {
  switch (expr->type) {
  case EXPR_ADD:
    return (void*)(
      (int)expression_run(ctx, ((Expression**)expr->params)[0]) +
      (int)expression_run(ctx, ((Expression**)expr->params)[1]));
  case EXPR_SUB:
    return (void*)(
      (int)expression_run(ctx, ((Expression**)expr->params)[0]) -
      (int)expression_run(ctx, ((Expression**)expr->params)[1]));
  case EXPR_MUL:
    return (void*)(
      (int)expression_run(ctx, ((Expression**)expr->params)[0]) *
      (int)expression_run(ctx, ((Expression**)expr->params)[1]));
  case EXPR_DIV:
    return (void*)(
      (int)expression_run(ctx, ((Expression**)expr->params)[0]) /
      (int)expression_run(ctx, ((Expression**)expr->params)[1]));
  case EXPR_EQUALS:
    return (void*)(
      expression_run(ctx, ((Expression**)expr->params)[0]) ==
      expression_run(ctx, ((Expression**)expr->params)[1]));
  case EXPR_NOT_EQUALS:
    return (void*)(
      expression_run(ctx, ((Expression**)expr->params)[0]) !=
      expression_run(ctx, ((Expression**)expr->params)[1]));
  case EXPR_LT:
    return (void*)(
      expression_run(ctx, ((Expression**)expr->params)[0]) <
      expression_run(ctx, ((Expression**)expr->params)[1]));
  case EXPR_GT:
    return (void*)(
      expression_run(ctx, ((Expression**)expr->params)[0]) >
      expression_run(ctx, ((Expression**)expr->params)[1]));
  case EXPR_AND:
    return (void*)(
      expression_run(ctx, ((Expression**)expr->params)[0]) &&
      expression_run(ctx, ((Expression**)expr->params)[1]));
  case EXPR_OR:
    return (void*)(
      expression_run(ctx, ((Expression**)expr->params)[0]) ||
      expression_run(ctx, ((Expression**)expr->params)[1]));
  case EXPR_NOT:
    return (void*)(
      ! expression_run(ctx, (Expression*)expr->params));
  case EXPR_INT_LITERAL:
    return (void*)expr->params;
  case EXPR_VAR_GET: {
    char *name = expr->params;
    Variable *v = context_variable_get(ctx, name);
    if (v != NULL)
      return v->value;
    break;
  }
  }
  return NULL;
}


void expression_destroy(Expression *expr) {
  switch (expr->type) {
  case EXPR_NOT:
    expression_destroy((Expression*)expr->params);
    break;
  case EXPR_ADD:
  case EXPR_SUB:
  case EXPR_MUL:
  case EXPR_DIV:
  case EXPR_EQUALS:
  case EXPR_NOT_EQUALS:
  case EXPR_LT:
  case EXPR_GT:
  case EXPR_AND:
  case EXPR_OR:
    expression_destroy(((Expression**)expr->params)[0]);
    expression_destroy(((Expression**)expr->params)[1]);
    break;
  }
}
