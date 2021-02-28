#include "vm.h"


Value *expression_run(Expression *expr, Context *ctx) {
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
  case EXPR_LT_EQ:
    return (void*)(
      expression_run(expr->param1, ctx) <=
      expression_run(expr->param2, ctx));
  case EXPR_GT_EQ:
    return (void*)(
      expression_run(expr->param1, ctx) >=
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
  case EXPR_LITERAL:
    return (Value*)expr->param1;
  case EXPR_VAR_GET: {
    char *name = expr->param1;
    Variable *v = context_variable_get(ctx, name);
    if (v != NULL)
      return v->value;
    // return v
    break;
  case EXPR_STRUCT_MEMBER_GET:
    break;
  }
  }
  return NULL;
}

