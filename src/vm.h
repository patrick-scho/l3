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


// Consts

static const Type TYPE_INT = { "int" };


// Expression

Expression *expression_create(ExpressionType type, void *params);
void *expression_run(Expression *expr);
void expression_destroy(Expression *expr);


// Statement

Statement *statement_create(StatementType type, void *params);
void *statement_run(Statement *stmt);
void statement_destroy(Statement *stmt);


// Context

Context *context_create();
void context_statement_add(Context *ctx, Statement *stmt);
void *context_run(Context *ctx);
void context_destroy(Context *ctx);


// Function

Function *function_create(const char *name, Type returnType, Type *parameterTypes);
void *function_run(Function *f);
void function_destroy(Function *f);