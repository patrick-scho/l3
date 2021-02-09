#pragma once

#include "vm.h"

Expression *expression_create(ExpressionType type, void *params) {
  Expression *result = malloc(sizeof *result);
  result->type = type;
  result->params = params;
  return result;
}


void *expression_run(Expression *expr) {
  switch (expr->type) {
  case EXPR_ADD:
    return (void*)(
      (int)expression_run(((Expression**)expr->params)[0]) +
      (int)expression_run(((Expression**)expr->params)[1]));
  case EXPR_INT_LITERAL:
    return (void*)expr->params;
  case EXPR_EQUALS:
    return (void*)(
      expression_run(((Expression**)expr->params)[0]) ==
      expression_run(((Expression**)expr->params)[1]));
  }
  return NULL;
}


void expression_destroy(Expression *expr) {
  switch (expr->type) {
  case EXPR_ADD:
  case EXPR_EQUALS:
    expression_destroy(((Expression**)expr->params)[0]);
    expression_destroy(((Expression**)expr->params)[1]);
    break;
  }
}