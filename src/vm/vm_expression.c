#include "vm.h"

#define EXPRESSION_CREATE(NAME, Name, name, args, assign)                               \
Expression##Name *expression_create##name##args {                                 \
  Expression##Name *result = (Expression##Name*)malloc(sizeof(Expression##Name)); \
  result->base = (Expression*)malloc(sizeof(Expression));                         \
  result->base->type = EXPR_##NAME;                                               \
  assign                                                                          \
  return result;                                                                  \
}

#define EXPRESSION_CREATE0(NAME, Name, name) \
EXPRESSION_CREATE(NAME, Name, name, (),)
#define EXPRESSION_CREATE1(NAME, Name, name, type1, arg1) \
EXPRESSION_CREATE(NAME, Name, name, (type1 arg1), result->arg1 = arg1;)
#define EXPRESSION_CREATE2(NAME, Name, name, type1, arg1, type2, arg2) \
EXPRESSION_CREATE(NAME, Name, name, (type1 arg1, type2 arg2), result->arg1 = arg1; result->arg2 = arg2;)

EXPRESSION_CREATE2(ADD,     Add,    _add,     Expression *, expr1, Expression *, expr2)
EXPRESSION_CREATE2(SUB,     Sub,    _sub,     Expression *, expr1, Expression *, expr2)
EXPRESSION_CREATE2(MUL,     Mul,    _mul,     Expression *, expr1, Expression *, expr2)
EXPRESSION_CREATE2(DIV,     Div,    _div,     Expression *, expr1, Expression *, expr2)
EXPRESSION_CREATE2(EQ,      Eq,     _eq,      Expression *, expr1, Expression *, expr2)
EXPRESSION_CREATE2(NOT_EQ,  NotEq,  _not_eq,  Expression *, expr1, Expression *, expr2)
EXPRESSION_CREATE2(LT,      Lt,     _lt,      Expression *, expr1, Expression *, expr2)
EXPRESSION_CREATE2(GT,      Gt,     _gt,      Expression *, expr1, Expression *, expr2)
EXPRESSION_CREATE2(LT_EQ,   LtEq,   _lt_eq,   Expression *, expr1, Expression *, expr2)
EXPRESSION_CREATE2(GT_EQ,   GtEq,   _gt_eq,   Expression *, expr1, Expression *, expr2)
EXPRESSION_CREATE1(NOT,     Not,    _not,     Expression *, expr1)
EXPRESSION_CREATE2(OR,      Or,     _or,      Expression *, expr1, Expression *, expr2)
EXPRESSION_CREATE2(AND,     And,    _and,     Expression *, expr1, Expression *, expr2)
EXPRESSION_CREATE1(INT_LIT, IntLit, _int_lit, long long, ll)
EXPRESSION_CREATE1(VAR_GET, VarGet, _var_get, char *, name)


#define EXPRESSION_CALL(name, expr, ...) \
switch (expr->type) {                    \
case EXPR_ADD:     expression_##name##_add    ((ExpressionAdd*)   expr, __VA_ARGS__); break; \
case EXPR_SUB:     expression_##name##_sub    ((ExpressionSub*)   expr, __VA_ARGS__); break; \
case EXPR_MUL:     expression_##name##_mul    ((ExpressionMul*)   expr, __VA_ARGS__); break; \
case EXPR_DIV:     expression_##name##_div    ((ExpressionDiv*)   expr, __VA_ARGS__); break; \
case EXPR_EQ:      expression_##name##_eq     ((ExpressionEq*)    expr, __VA_ARGS__); break; \
case EXPR_NOT_EQ:  expression_##name##_not_eq ((ExpressionNotEq*) expr, __VA_ARGS__); break; \
case EXPR_LT:      expression_##name##_lt     ((ExpressionLt*)    expr, __VA_ARGS__); break; \
case EXPR_GT:      expression_##name##_gt     ((ExpressionGt*)    expr, __VA_ARGS__); break; \
case EXPR_LT_EQ:   expression_##name##_lt_eq  ((ExpressionLtEq*)  expr, __VA_ARGS__); break; \
case EXPR_GT_EQ:   expression_##name##_gt_eq  ((ExpressionGtEq*)  expr, __VA_ARGS__); break; \
case EXPR_NOT:     expression_##name##_not    ((ExpressionNot*)   expr, __VA_ARGS__); break; \
case EXPR_OR:      expression_##name##_or     ((ExpressionOr*)    expr, __VA_ARGS__); break; \
case EXPR_AND:     expression_##name##_and    ((ExpressionAnd*)   expr, __VA_ARGS__); break; \
case EXPR_INT_LIT: expression_##name##_int_lit((ExpressionIntLit*)expr, __VA_ARGS__); break; \
case EXPR_VAR_GET: expression_##name##_var_get((ExpressionVarGet*)expr, __VA_ARGS__); break; \
}

void expression_destroy_add    (ExpressionAdd*    expr) {
  expression_destroy(expr->expr1);
  expression_destroy(expr->expr2);
}
void expression_destroy_sub    (ExpressionSub*    expr) {
  expression_destroy(expr->expr1);
  expression_destroy(expr->expr2);
}
void expression_destroy_mul    (ExpressionMul*    expr) {
  expression_destroy(expr->expr1);
  expression_destroy(expr->expr2);
}
void expression_destroy_div    (ExpressionDiv*    expr) {
  expression_destroy(expr->expr1);
  expression_destroy(expr->expr2);
}
void expression_destroy_eq     (ExpressionEq*     expr) {
  expression_destroy(expr->expr1);
  expression_destroy(expr->expr2);
}
void expression_destroy_not_eq (ExpressionNotEq*  expr) {
  expression_destroy(expr->expr1);
  expression_destroy(expr->expr2);
}
void expression_destroy_lt     (ExpressionLt*     expr) {
  expression_destroy(expr->expr1);
  expression_destroy(expr->expr2);
}
void expression_destroy_gt     (ExpressionGt*     expr) {
  expression_destroy(expr->expr1);
  expression_destroy(expr->expr2);
}
void expression_destroy_lt_eq  (ExpressionLtEq*   expr) {
  expression_destroy(expr->expr1);
  expression_destroy(expr->expr2);
}
void expression_destroy_gt_eq  (ExpressionGtEq*   expr) {
  expression_destroy(expr->expr1);
  expression_destroy(expr->expr2);
}
void expression_destroy_not    (ExpressionNot*    expr) {
  expression_destroy(expr->expr1);
}
void expression_destroy_or     (ExpressionOr*     expr) {
  expression_destroy(expr->expr1);
  expression_destroy(expr->expr2);
}
void expression_destroy_and    (ExpressionAnd*    expr) {
  expression_destroy(expr->expr1);
  expression_destroy(expr->expr2);
}
void expression_destroy_int_lit(ExpressionIntLit* expr) {
}
void expression_destroy_var_get(ExpressionVarGet* expr) {
}

Value expression_run(Expression *expr, Context *ctx) {
  EXPRESSION_CALL(run, expr, ctx)
}

void expression_destroy(Expression *expr) {
  EXPRESSION_CALL(destroy, expr)
  free(expr);
}
