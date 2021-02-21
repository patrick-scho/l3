#include "vm.h"

#define STATEMENT_CREATE(Name, name, args, assign)                             \
Statement##Name *statement_create##name args {                                 \
  Statement##Name *result = (Statement##Name*)malloc(sizeof(Statement##Name)); \
  result->base = (Statement*)malloc(sizeof(Statement));                        \
  assign                                                                       \
  return result;                                                               \
}

STATEMENT_CREATE(Expr,   _expr,   (Expression *expr), result->expr = expr; );
STATEMENT_CREATE(Return, _return, (Expression *expr), result->expr = expr; );
STATEMENT_CREATE(If,     _if,     (Expression *cond, Context *ctx), result->cond = cond; result->ctx = ctx; );
STATEMENT_CREATE(Else,   _else,   (Context *ctx), result->ctx = ctx; );
STATEMENT_CREATE(While,  _while,  (Expression *cond, Context *ctx), result->cond = cond; result->ctx = ctx; );
STATEMENT_CREATE(Ctx,    _ctx,    (Context *ctx), result->ctx = ctx; );
STATEMENT_CREATE(VarSet, _var_set, (char *name, Expression *expr), result->name = name; result->expr = expr; );

#undef STATEMENT_CREATE

Value *statement_run_expr   (StatementExpr   *stmt, Context *ctx) {
  return expression_run(stmt->expr, ctx);
}
Value *statement_run_return (StatementReturn *stmt, Context *ctx) {
  return expression_run(stmt->expr, ctx);
}
Value *statement_run_if     (StatementIf     *stmt, Context *ctx) {
  Value *v = expression_run(stmt->cond, ctx);
  if (type_match(v->type, &type_builtin_bool)) {
    if (*(bool*)v->value) {
      return context_run(stmt->ctx);
    }
    else {
      int index = context_get_statement_index(ctx, stmt);
      if (index < arrlen(ctx->statements) - 1) {
        if (ctx->statements[index + 1]->type == STMT_ELSE) {
          context_run(((StatementElse*)ctx->statements[index + 1])->ctx);
        }
      }
    }
  }
  return NULL;
}
Value *statement_run_while  (StatementWhile  *stmt, Context *ctx) {
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
Value *statement_run_ctx    (StatementCtx    *stmt, Context *ctx) {
  return context_run(stmt->ctx);
}
Value *statement_run_var_set (StatementVarSet *stmt, Context *ctx) {
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

#define STATEMENT_DESTROY(Name, name, destroy)        \
void statement_destroy##name(Statement##Name *stmt) { \
  destroy                                             \
  statement_destroy(stmt->base);                      \
  free(stmt);                                         \
}

STATEMENT_DESTROY(Expr,   _expr,   expression_destroy(stmt->expr); );
STATEMENT_DESTROY(Return, _return, expression_destroy(stmt->expr); );
STATEMENT_DESTROY(If,     _if,     expression_destroy(stmt->cond); context_destroy(stmt->ctx); );
STATEMENT_DESTROY(Else,   _else,   context_destroy(stmt->ctx); );
STATEMENT_DESTROY(While,  _while,  expression_destroy(stmt->cond); context_destroy(stmt->ctx); );
STATEMENT_DESTROY(Ctx,    _ctx,    context_destroy(stmt->ctx); );
STATEMENT_DESTROY(VarSet, _var_set, free(stmt->name); expression_destroy(stmt->expr); );

#undef STATEMENT_DESTROY

Value *statement_run(Statement *stmt, Context *ctx) {
  switch (stmt->type) {
  case STMT_EXPR:    statement_run_expr    (stmt, ctx);
  case STMT_RETURN:  statement_run_return  (stmt, ctx);
  case STMT_IF:      statement_run_if      (stmt, ctx);
  case STMT_WHILE:   statement_run_while   (stmt, ctx);
  case STMT_CTX:     statement_run_ctx     (stmt, ctx);
  case STMT_VAR_SET: statement_run_var_set (stmt, ctx);
  }
}


void statement_destroy(Statement *stmt) {
  switch (stmt->type) {
  case STMT_EXPR:    statement_destroy_expr    (stmt);
  case STMT_RETURN:  statement_destroy_return  (stmt);
  case STMT_IF:      statement_destroy_if      (stmt);
  case STMT_ELSE:    statement_destroy_else    (stmt);
  case STMT_WHILE:   statement_destroy_while   (stmt);
  case STMT_CTX:     statement_destroy_ctx     (stmt);
  case STMT_VAR_SET: statement_destroy_var_set (stmt);
  }
}
