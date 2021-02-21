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

Type *type_create(TypeType type);
void *type_alloc(Type *type);
uint type_get_byte_width(Type *type);
void type_destroy(Type *type);

// Value

typedef struct {
  Type *type;
  void *value;
} Value;

Value value_create(Type *type, void *value);

// Variable

typedef struct {
  char *name;
  Type *type;
  Value value;
} Variable;

Variable *variable_create(const char *name, Type *type, Value value);
void variable_destroy(Variable *var);

// Expression

typedef enum {
  EXPR_ADD, EXPR_SUB, EXPR_MUL, EXPR_DIV,
  EXPR_EQ, EXPR_NOT_EQ, EXPR_LT, EXPR_GT, EXPR_LT_EQ, EXPR_GT_EQ,
  EXPR_NOT, EXPR_OR, EXPR_AND,
  EXPR_INT_LIT, EXPR_VAR_GET
} ExpressionType;
typedef struct {
  ExpressionType type;
  void *param1;
  void *param2;
} Expression;

Value expression_run(Expression *expr, Context *ctx);
void expression_destroy(Expression *expr);

#define EXPRESSION(Name, name, members, arguments) \
typedef struct { Expression *base; members } Expression##Name; \
Expression *expression_create##name##arguments; \
Value expression_run##name(Expression##Name *expr); \
void expression_destroy##name(Expression##Name *expr);

#define EXPRESSION0(Name, name)             EXPRESSION(Name, name,,())
#define EXPRESSION1(Name, name, arg1)       EXPRESSION(Name, name,arg1;,(arg1))
#define EXPRESSION2(Name, name, arg1, arg2) EXPRESSION(Name, name,arg1; arg2;,(arg1, arg2))

EXPRESSION2(Add,    _add,     Expression *expr1, Expression *expr2)
EXPRESSION2(Sub,    _sub,     Expression *expr1, Expression *expr2)
EXPRESSION2(Mul,    _mul,     Expression *expr1, Expression *expr2)
EXPRESSION2(Div,    _div,     Expression *expr1, Expression *expr2)
EXPRESSION2(Eq,     _eq,      Expression *expr1, Expression *expr2)
EXPRESSION2(NotEq,  _not_eq,  Expression *expr1, Expression *expr2)
EXPRESSION2(Lt,     _lt,      Expression *expr1, Expression *expr2)
EXPRESSION2(Gt,     _gt,      Expression *expr1, Expression *expr2)
EXPRESSION2(LtEq,   _lt_eq,   Expression *expr1, Expression *expr2)
EXPRESSION2(GtEq,   _gt_eq,   Expression *expr1, Expression *expr2)
EXPRESSION1(Not,    _not,     Expression *expr1)
EXPRESSION2(Or,     _or,      Expression *expr1, Expression *expr2)
EXPRESSION2(And,    _and,     Expression *expr1, Expression *expr2)
EXPRESSION1(IntLit, _int_lit, long long ll)
EXPRESSION1(VarGet, _var_get, char *name)

#undef EXPRESSION

// Statement

typedef enum {
  STMT_EXPR, STMT_RETURN, STMT_IF, STMT_ELSE, STMT_WHILE, STMT_CTX,
  STMT_VAR_SET
} StatementType;
typedef struct {
  StatementType type;
} Statement;

Value statement_run(Statement *stmt, Context *ctx);
void statement_destroy(Statement *stmt);

#define STATEMENT(Name, name, members, arguments) \
typedef struct { Statement *base; members } Statement##Name; \
Statement *statement_create##name##arguments; \
Value statement_run##name(Statement##Name *stmt); \
void statement_destroy##name(Statement##Name *stmt);

#define STATEMENT0(Name, name)             STATEMENT(Name, name,,())
#define STATEMENT1(Name, name, arg1)       STATEMENT(Name, name,arg1;,(arg1))
#define STATEMENT2(Name, name, arg1, arg2) STATEMENT(Name, name,arg1; arg2;,(arg1, arg2))

STATEMENT1(Expr,   _expr,   Expression *expr)
STATEMENT1(Return, _return, Expression *expr)
STATEMENT1(If,     _if,     Expression *expr, Context *ctx)
STATEMENT1(Else,   _else,   Context *ctx)
STATEMENT1(While,  _while,  Expression *expr, Context *ctx)
STATEMENT1(Ctx,    _ctx,    Context *ctx)
STATEMENT1(VarSet, _varset, char *name, Expression *expr)

#undef STATEMENT

// Function

typedef struct {
  char *name;
  Context *ctx;
} Function;

Function *function_create(const char *name, Context *ctx);
Value function_run(Function *f);
void function_destroy(Function *f);

// Context

typedef struct Context {
  Context *parent;
  Variable **variables;
  Statement **statements;
  Function **functions;
  //Struct **structs;
} Context;

Context *context_create(Context *parent);
Variable *context_variable_get(Context *ctx, const char *name);
void context_set_parents(Context *ctx);
int context_get_statement_index(Context *ctx, Statement *stmt);
Value context_run(Context *ctx);
void context_destroy(Context *ctx);
