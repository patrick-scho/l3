#include "vm.h"

#include "util/memory.h"


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

void context_variable_set(Context *ctx, char *name, Value *value) {
  Variable *var = context_variable_get(ctx, name);
  if (var == NULL) {
    Variable *new_var = mem_init(Variable, ctx,
      .name = name,
      .value = value_copy(value));
    arrput(ctx->variables, new_var);
  }
  else {
    value_set(var->value, value);
  }
}


Function *context_function_get(Context *ctx, const char *name) {
  for (int i = 0; i < arrlen(ctx->functions); i++) {
    Function *f = ctx->functions[i];
    if (strcmp(f->name, name) == 0)
      return f;
  }
  if (ctx->parent != NULL)
    return context_function_get(ctx->parent, name);
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
