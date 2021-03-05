#pragma once

#include <stdbool.h>

#include <stb_ds.h>

typedef unsigned int uint;




// Forward Declarations

struct Type;
typedef struct Type Type;
struct Value;
typedef struct Value Value;
struct Variable;
typedef struct Variable Variable;
struct Expression;
typedef struct Expression Expression;
struct Statement;
typedef struct Statement Statement;
struct Function;
typedef struct Function Function;
struct Context;
typedef struct Context Context;

// Struct

// typedef struct Struct {
//   char *name;
//   struct {
//     char *key;
//     Type *value;
//   } *members;
// } Struct;

// void struct_free(Struct *s);

// Operators

typedef enum {
  UNARY_NOT,
  UNARY_COUNT
} UnaryOp;

typedef enum {
  BINARY_ADD, BINARY_SUB, BINARY_MUL, BINARY_DIV,
  BINARY_EQ, BINARY_NOT_EQ, BINARY_LT, BINARY_GT, BINARY_LT_EQ, BINARY_GT_EQ,
  BINARY_OR, BINARY_AND,
  BINARY_COUNT
} BinaryOp;

// Type

typedef enum {
  TYPE_NONE, TYPE_BASIC, TYPE_PTR, TYPE_ARRAY, TYPE_STRUCT
} TypeType;

typedef struct Type {
  TypeType type;
  char *name;
  uint byte_width;
  
  Type *ptr_type;

  Type *array_type;
  uint array_length;

  Value *(*unary_operators[UNARY_COUNT])(Value*);
  Value *(*binary_operators[BINARY_COUNT])(Value*, Value*);
  
  Value *(*copy)(Value*);

  //Struct *type_struct;
} Type;

void *type_alloc(Type *type, void *parent);
uint type_get_byte_width(Type *type);
bool type_match(Type *type1, Type *type2);

// Value

typedef struct Value {
  Type *type;
  void *value;
} Value;

Value *value_unary(UnaryOp op, Value *value);
Value *value_binary(BinaryOp op, Value *value1, Value *value2);
void   value_set(Value *v1, Value *v2);
Value *value_copy(Value *value);

// Variable

typedef struct Variable {
  char *name;
  Value *value;
} Variable;

// Expression

typedef enum {
  EXPR_UNARY, EXPR_BINARY,
  EXPR_LITERAL, EXPR_VAR_GET, EXPR_STRUCT_MEMBER_GET,
  EXPR_FUNC_CALL
} ExpressionType;
typedef struct Expression {
  ExpressionType type;
  union {
    struct { UnaryOp op; Expression *expr; } unary;
    struct { BinaryOp op; Expression *expr1; Expression *expr2; } binary;
    struct { Value *value; } literal;
    struct { char *name; } var_get;
    struct { char *name; Expression **params; } func_call;
  };
} Expression;

Value *expression_run(Expression *expr, Context *ctx);

// Statement

typedef enum {
  STMT_EXPR, STMT_RETURN, STMT_IF, STMT_ELSE, STMT_WHILE, STMT_CTX,
  STMT_VAR_SET, STMT_STRUCT_MEMBER_SET
} StatementType;
typedef struct Statement {
  StatementType type;
  union {
    struct { Expression *expr; } expr;
    struct { Context *ctx; } ctx;
    struct { Expression *expr; Context *ctx; } expr_ctx;
    struct { char *name; Expression *expr; } var_set;
  };
} Statement;

Value *statement_run(Statement *stmt, Context *ctx);

// Function

typedef struct Function {
  char *name;
  Context *ctx;
  Variable **parameters;
} Function;

Value *function_run(Function *f, Expression **parameters);

// Context

typedef struct Context {
  Context *parent;
  Variable **variables;
  Statement **statements;
  Function **functions;
  //Struct **structs;
} Context;

Variable *context_variable_get(Context *ctx, const char *name);
void      context_variable_set(Context *ctx, char *name, Value *value);
Function *context_function_get(Context *ctx, const char *name);
void      context_set_parents(Context *ctx);
int       context_get_statement_index(Context *ctx, Statement *stmt);
Value    *context_run(Context *ctx);
void      context_free(Context *ctx);
