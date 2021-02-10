#pragma once

#include "vm.h"

Statement *statement_create(StatementType type, void *params) {
  Statement *result = malloc(sizeof *result);
  result->type = type;
  result->params = params;
  return result;
}


void *statement_run(Statement *stmt) {
  switch (stmt->type) {
  case STMT_RETURN:
    return expression_run((Expression*)(stmt->params));
  case STMT_EXPR:
    expression_run((Expression*)(stmt->params));
    break;
  case STMT_IF:
    if (expression_run(((void**)stmt->params)[0]))
      return context_run(((void**)stmt->params)[1]);
    break;
  case STMT_CTX:
      return context_run((Context*)stmt->params);
    break;
  }
  return NULL;
}


void statement_destroy(Statement *stmt) {
  switch (stmt->type) {
  case STMT_EXPR:
  case STMT_RETURN:
    expression_destroy((Expression*)(stmt->params));
    break;
  case STMT_IF:
    expression_destroy(((void**)stmt->params)[0]);
    context_destroy(((void**)stmt->params)[1]);
    break;
  case STMT_CTX:
    context_destroy((Context*)(stmt->params));
    break;
  }
}