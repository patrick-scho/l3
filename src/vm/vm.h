#pragma once

#include <stdbool.h>

#include <stb_ds.h>

typedef unsigned int uint;

// Forward Declarations

struct Context;
typedef struct Context Context;
struct Type;
typedef struct Type Type;

// Struct

typedef struct Struct {
  char *name;
  struct {
    char *key;
    Type *value;
  } *members;
} Struct;

Struct *struct_create(char *name);
void struct_destroy(Struct *s);

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

  Struct *type_struct;
} Type;

Type *type_create(TypeType type);
void *type_alloc(Type *type);
uint type_get_byte_width(Type *type);
void type_destroy(Type *type);

// Value

typedef struct {
  Type *type;
  void *value;
} Value;

Value *value_create(Type *type, void *value);
void value_destroy(Value *value);

// Variable

typedef struct {
  char *name;
  Value *value;
} Variable;

Variable *variable_create(const char *name, Value *value);
void variable_destroy(Variable *var);

// Expression

typedef enum {
  EXPR_ADD, EXPR_SUB, EXPR_MUL, EXPR_DIV,
  EXPR_EQUALS, EXPR_NOT_EQUALS, EXPR_LT, EXPR_GT, EXPR_LT_EQ, EXPR_GT_EQ,
  EXPR_NOT, EXPR_OR, EXPR_AND,
  EXPR_INT_LITERAL, EXPR_VAR_GET, EXPR_STRUCT_MEMBER_GET
} ExpressionType;
typedef struct {
  ExpressionType type;
  void *param1;
  void *param2;
} Expression;

Expression *expression_create(ExpressionType type, void *param1, void *param2);
Value *expression_run(Expression *expr, Context *ctx);
void expression_destroy(Expression *expr);

// Statement

typedef enum {
  STMT_EXPR, STMT_RETURN, STMT_IF, STMT_ELSE, STMT_WHILE, STMT_CTX,
  STMT_VAR_SET, STMT_STRUCT_MEMBER_SET
} StatementType;
typedef struct {
  StatementType type;
  void *param1;
  void *param2;
} Statement;

Statement *statement_create(StatementType type, void *param1, void *param2);
Value *statement_run(Statement *stmt, Context *ctx);
void statement_destroy(Statement *stmt);

// Function

typedef struct {
  char *name;
  Context *ctx;
} Function;

Function *function_create(const char *name, Context *ctx);
Value *function_run(Function *f);
void function_destroy(Function *f);

// Context

typedef struct Context {
  Context *parent;
  Variable **variables;
  Statement **statements;
  Function **functions;
  Struct **structs;
} Context;

Context *context_create(Context *parent);
Variable *context_variable_get(Context *ctx, const char *name);
void context_set_parents(Context *ctx);
int context_get_statement_index(Context *ctx, Statement *stmt);
Value *context_run(Context *ctx);
void context_destroy(Context *ctx);
