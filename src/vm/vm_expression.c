#include "vm.h"

Expression *expression_create(ExpressionType type, void *param1, void *param2) {
  Expression *result = malloc(sizeof *result);
  result->type = type;
  result->param1 = param1;
  result->param2 = param2;
  return result;
}


void *expression_run(Expression *expr, Context *ctx) {
  switch (expr->type) {
  case EXPR_ADD:
    return (void*)(
      (int)expression_run(expr->param1, ctx) +
      (int)expression_run(expr->param2, ctx));
  case EXPR_SUB:
    return (void*)(
      (int)expression_run(expr->param1, ctx) -
      (int)expression_run(expr->param2, ctx));
  case EXPR_MUL:
    return (void*)(
      (int)expression_run(expr->param1, ctx) *
      (int)expression_run(expr->param2, ctx));
  case EXPR_DIV:
    return (void*)(
      (int)expression_run(expr->param1, ctx) /
      (int)expression_run(expr->param2, ctx));
  case EXPR_EQUALS:
    return (void*)(
      expression_run(expr->param1, ctx) ==
      expression_run(expr->param2, ctx));
  case EXPR_NOT_EQUALS:
    return (void*)(
      expression_run(expr->param1, ctx) !=
      expression_run(expr->param2, ctx));
  case EXPR_LT:
    return (void*)(
      expression_run(expr->param1, ctx) <
      expression_run(expr->param2, ctx));
  case EXPR_GT:
    return (void*)(
      expression_run(expr->param1, ctx) >
      expression_run(expr->param2, ctx));
  case EXPR_AND:
    return (void*)(
      expression_run(expr->param1, ctx) &&
      expression_run(expr->param2, ctx));
  case EXPR_OR:
    return (void*)(
      expression_run(expr->param1, ctx) ||
      expression_run(expr->param2, ctx));
  case EXPR_NOT:
    return (void*)(
      ! expression_run(expr->param1, ctx));
  case EXPR_INT_LITERAL:
    return (void*)expr->param1;
  case EXPR_VAR_GET: {
    char *name = expr->param1;
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
    expression_destroy((Expression*)expr->param1);
    break;
  case EXPR_VAR_GET:
    free((char*)expr->param1);
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
    expression_destroy(expr->param1);
    expression_destroy(expr->param2);
    break;
  }
  
  free(expr);
}
