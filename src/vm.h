#pragma once

#include <stdbool.h>

#include <stb_ds.h>

// Forward Declarations

struct Context;
typedef struct Context Context;

// Variable

typedef struct {
  char *name;
  void *value;
} Variable;

Variable *variable_create(const char *name, void *value);
void variable_destroy(Variable *var);

// Expression

typedef enum {
  EXPR_ADD, EXPR_SUB, EXPR_MUL, EXPR_DIV,
  EXPR_EQUALS, EXPR_NOT_EQUALS, EXPR_LT, EXPR_GT, EXPR_NOT,
  EXPR_OR, EXPR_AND,
  EXPR_INT_LITERAL, EXPR_VAR_GET
} ExpressionType;
typedef struct {
  ExpressionType type;
  void *params;
} Expression;

Expression *expression_create(ExpressionType type, void *params);
void *expression_run(Context *ctx, Expression *expr);
void expression_destroy(Expression *expr);

// Statement

typedef enum {
  STMT_EXPR, STMT_RETURN, STMT_IF, STMT_WHILE, STMT_CTX, STMT_VAR_SET
} StatementType;
typedef struct {
  StatementType type;
  void *params;
} Statement;

Statement *statement_create(StatementType type, void *params);
void *statement_run(Context *ctx, Statement *stmt);
void statement_destroy(Statement *stmt);

// Context

typedef struct Context {
  Context *parent;
  Variable **variables;
  Statement **statements;
} Context;

Context *context_create(Context *parent, Statement **stmts);
void context_statement_add(Context *ctx, Statement *stmt);
void context_variable_add(Context *ctx, Variable *var);
Variable *context_variable_get(Context *ctx, const char *name);
void *context_run(Context *ctx);
void context_destroy(Context *ctx);

// Function

typedef struct {
  char *name;
  Context *ctx;
} Function;

Function *function_create(Context *ctx, const char *name);
void *function_run(Function *f);
void function_destroy(Function *f);
