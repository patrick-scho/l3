#include "vm.h"



Variable *context_variable_get(Context *ctx, const char *name) {
  for (int i = 0; i < arrlen(ctx->variables); i++) {
    Variable *v = ctx->variables[i];
    if (strcmp(v->name, name) == 0)
      return v;
  }
  if (ctx->parent != NULL)
    return context_variable_get(ctx->parent, name);
  return NULL;
}


void context_set_parents(Context *ctx) {
  for (int i = 0; i < arrlen(ctx->statements); i++) {
    Statement *stmt = ctx->statements[i];
    if (
      stmt->type == STMT_CTX ||
      stmt->type == STMT_ELSE) {
      Context *c = stmt->ctx.ctx;
      c->parent = ctx;
      context_set_parents(c);
    }
    if (
      stmt->type == STMT_IF ||
      stmt->type == STMT_WHILE) {
      Context *c = stmt->expr_ctx.ctx;
      c->parent = ctx;
      context_set_parents(c);
    }
  }
}


int context_get_statement_index(Context *ctx, Statement *stmt) {
  for (int i = 0; i < arrlen(ctx->statements); i++)
    if (ctx->statements[i] == stmt)
      return i;
  return -1;
}


Value *context_run(Context *ctx) {
  for (int i = 0; i < arrlen(ctx->statements); i++) {
    Value *result = statement_run(ctx->statements[i], ctx);
    if (result != NULL)
      return result;
  }
  return NULL;
}


void context_free(Context *ctx) {
  arrfree(ctx->statements);
  arrfree(ctx->variables);
  arrfree(ctx->functions);
  //arrfree(ctx->structs);
}
