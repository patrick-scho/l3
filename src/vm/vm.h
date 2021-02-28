#pragma once

#include <stdio.h>
#include <stdbool.h>
#include <setjmp.h>

#include <stb_ds.h>

typedef unsigned int uint;



static jmp_buf error_jmp_buf;
static void error(const char *msg) {
  fprintf(stderr, "Error: %s\n", msg);
  longjmp(error_jmp_buf, 1);
}



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

// void struct_free(Struct *s);

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

void *type_alloc(Type *type);
uint type_get_byte_width(Type *type);

// Value

typedef struct {
  Type *type;
  void *value;
} Value;

// Variable

typedef struct {
  char *name;
  Value *value;
} Variable;

// Expression

typedef enum {
  EXPR_ADD, EXPR_SUB, EXPR_MUL, EXPR_DIV,
  EXPR_EQUALS, EXPR_NOT_EQUALS, EXPR_LT, EXPR_GT, EXPR_LT_EQ, EXPR_GT_EQ,
  EXPR_NOT, EXPR_OR, EXPR_AND,
  EXPR_LITERAL, EXPR_VAR_GET, EXPR_STRUCT_MEMBER_GET
} ExpressionType;
typedef struct {
  ExpressionType type;
  void *param1;
  void *param2;
} Expression;

Value *expression_run(Expression *expr, Context *ctx);

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

Value *statement_run(Statement *stmt, Context *ctx);

// Function

typedef struct {
  char *name;
  Context *ctx;
} Function;

// Context

typedef struct Context {
  Context *parent;
  Variable **variables;
  Statement **statements;
  Function **functions;
  //Struct **structs;
} Context;

Variable *context_variable_get(Context *ctx, const char *name);
void context_set_parents(Context *ctx);
int context_get_statement_index(Context *ctx, Statement *stmt);
Value *context_run(Context *ctx);
void context_free(Context *ctx);
