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
struct Expression;
typedef struct Expression Expression;

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
void *type_alloc(Type *type);
uint type_get_byte_width(Type *type);
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

Variable *variable_create(char *name, Type *type, Value *value);
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
  ExpressionUnaryType type;
  Expression *expr1;
} ExpressionUnary;
typedef struct {
  ExpressionBinaryType type;
  Expression *expr1;
  Expression *expr2;
} ExpressionBinary;
typedef struct {
  ExpressionLiteralType type;
  void *value;
} ExpressionLiteral;
typedef struct {
  char *name;
} ExpressionVarGet;

typedef struct Expression {
  ExpressionType type;
  union {
    ExpressionUnary *expr_unary;
    ExpressionBinary *expr_binary;
    ExpressionLiteral *expr_literal;
    ExpressionVarGet *expr_var_get;
  };
} Expression;

Value *expression_run(Expression *expr, Context *ctx);
void   expression_destroy(Expression *expr);

Expression *expression_create_unary  (ExpressionUnaryType type, Expression *expr1);
Expression *expression_create_binary (ExpressionBinaryType type, Expression *expr1, Expression *expr2);
Expression *expression_create_literal(ExpressionLiteralType type, void *value);
Expression *expression_create_var_get(char *name);

Value *expression_run_unary  (ExpressionUnary   *expr, Context *ctx);
Value *expression_run_binary (ExpressionBinary  *expr, Context *ctx);
Value *expression_run_literal(ExpressionLiteral *expr, Context *ctx);
Value *expression_run_var_get(ExpressionVarGet  *expr, Context *ctx);

// Statement

typedef struct {
  Expression *expr;
} StatementExpr;
typedef struct {
  Expression *expr;
} StatementReturn;
typedef struct {
  Expression *cond;
  Context *ctx;
} StatementIf;
typedef struct {
  Context *ctx;
} StatementElse;
typedef struct {
  Expression *cond;
  Context *ctx;
} StatementWhile;
typedef struct {
  Context *ctx;
} StatementCtx;
typedef struct {
  char *name;
  Expression *expr;
} StatementVarSet;

typedef enum {
  STMT_EXPR, STMT_RETURN, STMT_IF, STMT_ELSE, STMT_WHILE, STMT_CTX,
  STMT_VAR_SET
} StatementType;
typedef struct {
  StatementType type;
  union {
    StatementExpr *stmt_expr;
    StatementReturn *stmt_return;
    StatementIf *stmt_if;
    StatementElse *stmt_else;
    StatementWhile *stmt_while;
    StatementCtx *stmt_ctx;
    StatementVarSet *stmt_var_set;
  };
} Statement;

Value *statement_run(Statement *stmt, Context *ctx);
void   statement_destroy(Statement *stmt);

Statement *statement_create_expr   (Expression *expr);
Statement *statement_create_return (Expression *expr);
Statement *statement_create_if     (Expression *cond, Context *ctx);
Statement *statement_create_else   (Context *ctx);
Statement *statement_create_while  (Expression *cond, Context *ctx);
Statement *statement_create_ctx    (Context *ctx);
Statement *statement_create_var_set (char *name, Expression *expr);

Value *statemtent_run_expr   (StatementExpr   *stmt, Context *ctx);
Value *statemtent_run_return (StatementReturn *stmt, Context *ctx);
Value *statemtent_run_if     (StatementIf     *stmt, Context *ctx);
Value *statemtent_run_while  (StatementWhile  *stmt, Context *ctx);
Value *statemtent_run_ctx    (StatementCtx    *stmt, Context *ctx);
Value *statemtent_run_var_set (StatementVarSet *stmt, Context *ctx);


// Function

typedef struct {
  char *name;
  Context *ctx;
} Function;

Function *function_create(char *name, Context *ctx);
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
Variable *context_variable_get(Context *ctx, char *name);
void      context_set_parents(Context *ctx);
int       context_get_statement_index(Context *ctx, Statement *stmt);
Value    *context_run(Context *ctx);
void      context_destroy(Context *ctx);
