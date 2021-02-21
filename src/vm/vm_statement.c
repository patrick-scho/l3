#include "vm.h"

#define STATEMENT_CREATE(NAME, Name, name, args, assign)                  \
Statement *statement_create##name args {                                  \
  Statement *result = (Statement*)malloc(sizeof(Statement));              \
  result->stmt##name = (Statement##Name*)malloc(sizeof(Statement##Name)); \
  result->type = STMT_##NAME;                                             \
  assign                                                                  \
  return result;                                                          \
}

STATEMENT_CREATE(EXPR,    Expr,   _expr,   (Expression *expr), result->stmt_expr->expr = expr; );
STATEMENT_CREATE(RETURN,  Return, _return, (Expression *expr), result->stmt_return->expr = expr; );
STATEMENT_CREATE(IF,      If,     _if,     (Expression *cond, Context *ctx), result->stmt_if->cond = cond; result->stmt_if->ctx = ctx; );
STATEMENT_CREATE(ELSE,    Else,   _else,   (Context *ctx), result->stmt_else->ctx = ctx; );
STATEMENT_CREATE(WHILE,   While,  _while,  (Expression *cond, Context *ctx), result->stmt_while->cond = cond; result->stmt_while->ctx = ctx; );
STATEMENT_CREATE(CTX,     Ctx,    _ctx,    (Context *ctx), result->stmt_ctx->ctx = ctx; );
STATEMENT_CREATE(VAR_SET, VarSet, _var_set, (char *name, Expression *expr), result->stmt_var_set->name = name; result->stmt_var_set->expr = expr; );

#undef STATEMENT_CREATE

Value *statement_run_expr(StatementExpr *stmt, Context *ctx) {
  expression_run(stmt->expr, ctx);
  return NULL;
}
Value *statement_run_return(StatementReturn *stmt, Context *ctx) {
  return expression_run(stmt->expr, ctx);
}
Value *statement_run_if(StatementIf     *stmt, Context *ctx) {
  Value *v = expression_run(stmt->cond, ctx);
  if (type_match(v->type, &type_builtin_bool)) {
    if (*(bool*)v->value) {
      return context_run(stmt->ctx);
    }
    else {
      int index = context_get_statement_index(ctx, (Statement*)(((StatementType*)stmt)-1));
      if (index < arrlen(ctx->statements) - 1) {
        if (ctx->statements[index + 1]->type == STMT_ELSE) {
          return context_run(((StatementElse*)ctx->statements[index + 1])->ctx);
        }
      }
    }
  }
  return NULL;
}
Value *statement_run_while(StatementWhile *stmt, Context *ctx) {
  Value *v = expression_run(stmt->cond, ctx);
  if (type_match(v->type, &type_builtin_bool)) {
    while (*(bool*)v->value) {
      Value *result = context_run(stmt->ctx);
      if (result != NULL)
        return result;
      v = expression_run(stmt->cond, ctx);
    }
  }
  return NULL;
}
Value *statement_run_ctx(StatementCtx *stmt, Context *ctx) {
  return context_run(stmt->ctx);
}
Value *statement_run_var_set(StatementVarSet *stmt, Context *ctx) {
  Value *v = expression_run(stmt->expr, ctx);

  Variable *var = context_variable_get(ctx, stmt->name);

  if (type_match(var->type, v->type)) {
    value_destroy(var->value);
    var->value = v;
  }
  else {
    value_destroy(v);
  }

  return NULL;
}

Value *statement_run(Statement *stmt, Context *ctx) {
  Value *result = NULL;
  switch (stmt->type) {
  case STMT_EXPR:    result = statement_run_expr    (stmt->stmt_expr, ctx);    break;
  case STMT_RETURN:  result = statement_run_return  (stmt->stmt_return, ctx);  break;
  case STMT_IF:      result = statement_run_if      (stmt->stmt_if, ctx);      break;
  case STMT_WHILE:   result = statement_run_while   (stmt->stmt_while, ctx);   break;
  case STMT_CTX:     result = statement_run_ctx     (stmt->stmt_ctx, ctx);     break;
  case STMT_VAR_SET: result = statement_run_var_set (stmt->stmt_var_set, ctx); break;
  }
  return result;
}


void statement_destroy(Statement *stmt) {
  switch (stmt->type) {
  case STMT_EXPR:
    expression_destroy(stmt->stmt_expr->expr);
    break;
  case STMT_RETURN:
    expression_destroy(stmt->stmt_return->expr);
    break;
  case STMT_IF:
    expression_destroy(stmt->stmt_if->cond);
    context_destroy(stmt->stmt_if->ctx);
    break;
  case STMT_ELSE:
    context_destroy(stmt->stmt_else->ctx);
    break;
  case STMT_WHILE:
    expression_destroy(stmt->stmt_while->cond);
    context_destroy(stmt->stmt_while->ctx);
    break;
  case STMT_CTX:
    context_destroy(stmt->stmt_ctx->ctx);
    break;
  case STMT_VAR_SET:
    free(stmt->stmt_var_set->name);
    expression_destroy(stmt->stmt_var_set->expr);
    break;

  }
  free(stmt);
}
