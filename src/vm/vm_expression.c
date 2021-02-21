#include "vm.h"

ExpressionUnary *expression_create_unary(ExpressionUnaryType type, Expression *expr1) {
  ExpressionUnary *result = (ExpressionUnary*)malloc(sizeof(ExpressionUnary));

  result->base = (Expression*)malloc(sizeof(Expression));
  result->base->type = EXPR_UNARY;
  result->type = type;
  result->expr1 = expr1;

  return result;
}
ExpressionBinary *expression_create_binary(ExpressionBinaryType type, Expression *expr1, Expression *expr2) {
  ExpressionBinary *result = (ExpressionBinary*)malloc(sizeof(ExpressionBinary));

  result->base = (Expression*)malloc(sizeof(Expression));
  result->base->type = EXPR_BINARY;
  result->type = type;
  result->expr1 = expr1;
  result->expr2 = expr2;

  return result;
}
ExpressionLiteral *expression_create_literal(ExpressionLiteralType type, void *value) {
  ExpressionLiteral *result = (ExpressionLiteral*)malloc(sizeof(ExpressionLiteral));

  result->base = (Expression*)malloc(sizeof(Expression));
  result->base->type = EXPR_LITERAL;
  result->type = type;
  result->value = value;

  return result;
}
ExpressionVarGet *expression_create_var_get(char *name) {
  ExpressionVarGet *result = (ExpressionVarGet*)malloc(sizeof(ExpressionVarGet));

  result->base = (Expression*)malloc(sizeof(Expression));
  result->base->type = EXPR_VAR_GET;
  result->name = name;

  return result;
}


Value *expression_run_unary(ExpressionUnary *expr, Context *ctx) {
  Value *val = expression_run(expr->expr1, ctx);

  Value *result;

  if (type_match(val->type, &type_builtin_bool)) {
    bool *v = (bool*)val->value;

    switch (expr->type) {
    case EXPR_UNARY_NOT: result = value_create(&type_builtin_bool, !(*v)); break;
    }
  }
  else {
    error("unexpected type");
  }

  value_destroy(val);

  return result;
}
Value *expression_run_binary(ExpressionBinary *expr, Context *ctx) {
  Value *val1 = expression_run(expr->expr1, ctx);
  Value *val2 = expression_run(expr->expr2, ctx);

  Value *result;

  if (!type_match(val1->type, val2->type))
    error("type mismatch");

  if (type_match(val1->type, &type_builtin_int)) {
    s64 *v1 = (s64*)val1->value;
    s64 *v2 = (s64*)val2->value;
    switch (expr->type) {
    case EXPR_BINARY_ADD:    result = value_create(&type_builtin_int,  alloc_s64(*v1 +  *v2)); break;
    case EXPR_BINARY_SUB:    result = value_create(&type_builtin_int,  alloc_s64(*v1 -  *v2)); break;
    case EXPR_BINARY_MUL:    result = value_create(&type_builtin_int,  alloc_s64(*v1 *  *v2)); break;
    case EXPR_BINARY_DIV:    result = value_create(&type_builtin_int,  alloc_s64(*v1 /  *v2)); break;
    case EXPR_BINARY_EQ:     result = value_create(&type_builtin_bool, alloc_bool(*v1 == *v2)); break;
    case EXPR_BINARY_NOT_EQ: result = value_create(&type_builtin_bool, alloc_bool(*v1 != *v2)); break;
    case EXPR_BINARY_LT:     result = value_create(&type_builtin_bool, alloc_bool(*v1 <  *v2)); break;
    case EXPR_BINARY_GT:     result = value_create(&type_builtin_bool, alloc_bool(*v1 >  *v2)); break;
    case EXPR_BINARY_LT_EQ:  result = value_create(&type_builtin_bool, alloc_bool(*v1 <= *v2)); break;
    case EXPR_BINARY_GT_EQ:  result = value_create(&type_builtin_bool, alloc_bool(*v1 >= *v2)); break;
    }
  }
  else if (type_match(val1->type, &type_builtin_bool)) {
    bool *v1 = (bool*)val1->value;
    bool *v2 = (bool*)val2->value;
    switch (expr->type) {
    case EXPR_BINARY_EQ:     result = value_create(&type_builtin_bool, alloc_bool(*v1 == *v2)); break;
    case EXPR_BINARY_NOT_EQ: result = value_create(&type_builtin_bool, alloc_bool(*v1 != *v2)); break;
    case EXPR_BINARY_LT:     result = value_create(&type_builtin_bool, alloc_bool(*v1 <  *v2)); break;
    case EXPR_BINARY_GT:     result = value_create(&type_builtin_bool, alloc_bool(*v1 >  *v2)); break;
    case EXPR_BINARY_LT_EQ:  result = value_create(&type_builtin_bool, alloc_bool(*v1 <= *v2)); break;
    case EXPR_BINARY_GT_EQ:  result = value_create(&type_builtin_bool, alloc_bool(*v1 >= *v2)); break;
    }
  }
  else {
    error("unexpected type");
  }
  
  value_destroy(val1);
  value_destroy(val2);

  return result;
}
Value *expression_run_literal(ExpressionLiteral *expr, Context *ctx) {
  Value *result;

  switch (expr->type) {
  case EXPR_LITERAL_INT:
    result = value_create(&type_builtin_int, alloc_s64((s64)expr->value));
    break;
  }

  return result;
}
Value *expression_run_var_get(ExpressionVarGet *expr, Context *ctx) {
  Variable *var = context_variable_get(ctx, expr->name);

  if (var == NULL)
    error("unknown variable");

  return var->value;
}


Value *expression_run(Expression *expr, Context *ctx) {
  switch (expr->type) {
  case EXPR_UNARY:   return expression_run_unary(expr, ctx);
  case EXPR_BINARY:  return expression_run_binary(expr, ctx);
  case EXPR_LITERAL: return expression_run_literal(expr, ctx);
  case EXPR_VAR_GET: return expression_run_var_get(expr, ctx);
  }
  return NULL;
}



void expression_destroy(Expression *expr) {
  switch (expr->type) {
  case EXPR_UNARY: {
    ExpressionUnary *e = expr;
    expression_destroy(e->expr1);
    break;
  }
  case EXPR_BINARY: {
    ExpressionBinary *e = expr;
    expression_destroy(e->expr1);
    expression_destroy(e->expr2);
    break;
  }
  case EXPR_LITERAL: {
    ExpressionLiteral *e = expr;
    free(e->value);
    break;
  }
  case EXPR_VAR_GET: {
    ExpressionVarGet *e = expr;
    free(e->name);
    break;
  }
  }
  free(expr);
}
