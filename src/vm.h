#pragma once

#include <stdbool.h>

#include <vec.h>



// Structs

typedef struct {
  const char *name;
} Type;

typedef enum {
  EXPR_ADD, EXPR_EQUALS, EXPR_INT_LITERAL
} ExpressionType;
typedef struct {
  ExpressionType type;
  void *params;
} Expression;

typedef enum {
  STMT_EXPR, STMT_RETURN, STMT_IF
} StatementType;
typedef struct {
  StatementType type;
  void *params;
} Statement;

typedef vec_t(Statement*) Statement_vec;
typedef vec_t(Type) Type_vec;

typedef struct {
  Statement_vec statements;
} Context;

typedef struct {
  Type returnType;
  Type_vec parameterTypes;
  Context *ctx;
} Function;



// Forward declarations

void *context_run(Context *ctx);



// Consts

static const Type TYPE_INT = { "int" };



// Expression

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



// Statement

Statement *statement_create(StatementType type, void *params) {
  Statement *result = malloc(sizeof *result);
  result->type = type;
  result->params = params;
  return result;
}
void *statement_run(Statement *stmt) {
  switch (stmt->type) {
  case STMT_EXPR:
    expression_run((Expression*)(stmt->params));
    return NULL;
  case STMT_RETURN:
    return expression_run((Expression*)(stmt->params));
  case STMT_IF:
    if (expression_run(((void**)stmt->params)[0]))
      return context_run(((void**)stmt->params)[1]);
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
  }
}



// Context

Context *context_create() {
  Context *result = malloc(sizeof *result);

  vec_init(&result->statements);

  return result;
}
void context_statement_add(Context *ctx, Statement *stmt) {
  vec_push(&ctx->statements, stmt);
}
void *context_run(Context *ctx) {
  Statement *stmt; int i;
  vec_foreach (&ctx->statements, stmt, i) {
    void *result = statement_run(stmt);
    if (result != NULL)
      return result;
  }
  return NULL;
}
void context_destroy(Context *ctx) {
  Statement *stmt; int i;
  vec_foreach(&ctx->statements, stmt, i) {
    statement_destroy(stmt);
  }
  vec_deinit(&ctx->statements);
  free(ctx);
}



// Function

Function *function_create(const char *name, Type returnType, Type *parameterTypes) {
  Function *result = malloc(sizeof *result);
  result->returnType = returnType;

  vec_init(&result->parameterTypes);
  for (int i = 0; parameterTypes[i].name != NULL; i++)
    vec_push(&result->parameterTypes, parameterTypes[i]);

  result->ctx = context_create();

  return result;
}
void *function_run(Function *f) {
  return context_run(f->ctx);
}
void function_destroy(Function *f) {
  context_destroy(f->ctx);
  vec_deinit(&f->parameterTypes);
  free(f);
}
