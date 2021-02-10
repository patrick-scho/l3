#pragma once

#include <stdbool.h>

#include <stb_ds.h>

// Expression

typedef enum {
  EXPR_ADD, EXPR_SUB, EXPR_MUL, EXPR_DIV,
  EXPR_EQUALS, EXPR_NOT_EQUALS, EXPR_LT, EXPR_GT, EXPR_NOT,
  EXPR_INT_LITERAL
} ExpressionType;
typedef struct {
  ExpressionType type;
  void *params;
} Expression;

Expression *expression_create(ExpressionType type, void *params);
void *expression_run(Expression *expr);
void expression_destroy(Expression *expr);

// Statement

typedef enum {
  STMT_EXPR, STMT_RETURN, STMT_IF, STMT_CTX
} StatementType;
typedef struct {
  StatementType type;
  void *params;
} Statement;

Statement *statement_create(StatementType type, void *params);
void *statement_run(Statement *stmt);
void statement_destroy(Statement *stmt);

// Context

typedef struct {
  Statement **statements;
} Context;

Context *context_create();
void context_statement_add(Context *ctx, Statement *stmt);
void *context_run(Context *ctx);
void context_destroy(Context *ctx);

// Function

typedef struct {
  Context *ctx;
} Function;

Function *function_create(const char *name);
void *function_run(Function *f);
void function_destroy(Function *f);
