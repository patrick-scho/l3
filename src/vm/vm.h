#pragma once

#include <stdio.h>
#include <stdbool.h>
#include <setjmp.h>

#include <stb_ds.h>

typedef unsigned int uint;
typedef signed long s64;

static jmp_buf error_jmp_buf;
static void error(const char *msg) {
  fprintf(stderr, "Error: %s\n", msg);
  longjmp(error_jmp_buf, 1);
}

#define ALLOC(type)                           \
static void *alloc_##type(type v) {                  \
  type *result = (type*)malloc(sizeof(type)); \
  *result = v;                                \
  return (void*)result;                       \
}
ALLOC(s64)
ALLOC(bool)

// Forward Declarations

struct Context;
typedef struct Context Context;
struct Type;
typedef struct Type Type;

// Struct

// typedef struct Struct {
//   char *name;
//   struct {
//     char *key;
//     Type *value;
//   } *members;
// } Struct;

// Struct *struct_create(char *name);
// void struct_destroy(Struct *s);

// Type

typedef enum {
  TYPE_NONE, TYPE_BASIC, TYPE_PTR, TYPE_ARRAY, TYPE_STRUCT
} TypeType;

typedef struct Type {
  TypeType type;
  uint byte_width;
  
  Type *ptr_type;

  Type *array_type;
  uint array_length;

  //Struct *type_struct;
} Type;

bool type_match(Type *t1, Type *t2);

#include "vm_builtin_types.h"

// Type *type_create(TypeType type);
// void *type_alloc(Type *type);
// uint type_get_byte_width(Type *type);
// void type_destroy(Type *type);

// Value

typedef struct {
  Type *type;
  void *value;
} Value;

Value *value_create(Type *type, void *value);
void   value_destroy(Value *value);

// Variable

typedef struct {
  char *name;
  Type *type;
  Value *value;
} Variable;

Variable *variable_create(const char *name, Type *type, Value *value);
void      variable_destroy(Variable *var);

// Expression

typedef enum {
  EXPR_UNARY, EXPR_BINARY, EXPR_LITERAL, EXPR_VAR_GET
} ExpressionType;
typedef enum {
  EXPR_UNARY_NOT
} ExpressionUnaryType;
typedef enum {
  EXPR_BINARY_ADD, EXPR_BINARY_SUB, EXPR_BINARY_MUL, EXPR_BINARY_DIV,
  EXPR_BINARY_EQ, EXPR_BINARY_NOT_EQ, EXPR_BINARY_LT, EXPR_BINARY_GT, EXPR_BINARY_LT_EQ, EXPR_BINARY_GT_EQ,
  EXPR_BINARY_OR, EXPR_BINARY_AND,
} ExpressionBinaryType;
typedef enum {
  EXPR_LITERAL_INT
} ExpressionLiteralType;

typedef struct {
  ExpressionType type;
  void *param1;
  void *param2;
} Expression;

Value *expression_run(Expression *expr, Context *ctx);
void   expression_destroy(Expression *expr);

typedef struct {
  Expression *base;
  ExpressionUnaryType type;
  Expression *expr1;
} ExpressionUnary;
typedef struct {
  Expression *base;
  ExpressionBinaryType type;
  Expression *expr1;
  Expression *expr2;
} ExpressionBinary;
typedef struct {
  Expression *base;
  ExpressionLiteralType type;
  void *value;
} ExpressionLiteral;
typedef struct {
  Expression *base;
  char *name;
} ExpressionVarGet;

ExpressionUnary   *expression_create_unary  (ExpressionUnaryType type, Expression *expr1);
ExpressionBinary  *expression_create_binary (ExpressionBinaryType type, Expression *expr1, Expression *expr2);
ExpressionLiteral *expression_create_literal(ExpressionLiteralType type, void *value);
ExpressionVarGet  *expression_create_var_get(char *name);

Value *expression_run_unary  (ExpressionUnary   *expr, Context *ctx);
Value *expression_run_binary (ExpressionBinary  *expr, Context *ctx);
Value *expression_run_literal(ExpressionLiteral *expr, Context *ctx);
Value *expression_run_var_get(ExpressionVarGet  *expr, Context *ctx);

void expression_destroy_unary  (ExpressionUnary   *expr);
void expression_destroy_binary (ExpressionBinary  *expr);
void expression_destroy_literal(ExpressionLiteral *expr);
void expression_destroy_var_get(ExpressionVarGet  *expr);

// Statement

typedef enum {
  STMT_EXPR, STMT_RETURN, STMT_IF, STMT_ELSE, STMT_WHILE, STMT_CTX,
  STMT_VAR_SET
} StatementType;
typedef struct {
  StatementType type;
} Statement;

Value *statement_run(Statement *stmt, Context *ctx);
void   statement_destroy(Statement *stmt);

typedef struct {
  Statement *base;
  Expression *expr;
} StatementExpr;
typedef struct {
  Statement *base;
  Expression *expr;
} StatementReturn;
typedef struct {
  Statement *base;
  Expression *cond;
  Context *ctx;
} StatementIf;
typedef struct {
  Statement *base;
  Context *ctx;
} StatementElse;
typedef struct {
  Statement *base;
  Expression *cond;
  Context *ctx;
} StatementWhile;
typedef struct {
  Statement *base;
  Context *ctx;
} StatementCtx;
typedef struct {
  Statement *base;
  char *name;
  Expression *expr;
} StatementVarSet;

StatementExpr   *statement_create_expr   (Expression *expr);
StatementReturn *statement_create_return (Expression *expr);
StatementIf     *statement_create_if     (Expression *cond, Context *ctx);
StatementElse   *statement_create_else   (Context *ctx);
StatementWhile  *statement_create_while  (Expression *cond, Context *ctx);
StatementCtx    *statement_create_ctx    (Context *ctx);
StatementVarSet *statement_create_var_set (char *name, Expression *expr);

Value *statemtent_run_expr   (StatementExpr   *stmt, Context *ctx);
Value *statemtent_run_return (StatementReturn *stmt, Context *ctx);
Value *statemtent_run_if     (StatementIf     *stmt, Context *ctx);
Value *statemtent_run_while  (StatementWhile  *stmt, Context *ctx);
Value *statemtent_run_ctx    (StatementCtx    *stmt, Context *ctx);
Value *statemtent_run_var_set (StatementVarSet *stmt, Context *ctx);

void statemtent_destroy_expr   (StatementExpr   *stmt);
void statemtent_destroy_return (StatementReturn *stmt);
void statemtent_destroy_if     (StatementIf     *stmt);
void statemtent_destroy_else   (StatementElse   *stmt);
void statemtent_destroy_while  (StatementWhile  *stmt);
void statemtent_destroy_ctx    (StatementCtx    *stmt);
void statemtent_destroy_var_set (StatementVarSet *stmt);

// Function

typedef struct {
  char *name;
  Context *ctx;
} Function;

Function *function_create(const char *name, Context *ctx);
Value    *function_run(Function *f);
void      function_destroy(Function *f);

// Context

typedef struct Context {
  Context *parent;
  Variable **variables;
  Statement **statements;
  Function **functions;
  //Struct **structs;
} Context;

Context  *context_create(Context *parent);
Variable *context_variable_get(Context *ctx, const char *name);
void      context_set_parents(Context *ctx);
int       context_get_statement_index(Context *ctx, Statement *stmt);
Value    *context_run(Context *ctx);
void      context_destroy(Context *ctx);
