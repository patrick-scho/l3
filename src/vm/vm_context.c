#include "vm.h"

Context *context_create(Context *parent, Statement **stmts) {
  Context *result = malloc(sizeof *result);

  result->parent = parent;

  result->statements = NULL;
  if (stmts != NULL)
    for (int i = 0; stmts[i] != NULL; i++)
      arrput(result->statements, stmts[i]);

  result->variables = NULL;

  result->functions = NULL;

  return result;
}


void context_statement_add(Context *ctx, Statement *stmt) {
  arrput(ctx->statements, stmt);
}


void context_variable_add(Context *ctx, Variable *var) {
  arrput(ctx->variables, var);
}

void context_function_add(Context *ctx, Function *fn) {
  arrput(ctx->functions, fn);
}


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
    if (stmt->type == STMT_CTX || stmt->type == STMT_IF || stmt->type == STMT_WHILE) {
      Context *c = stmt->param2;
      c->parent = ctx;
      setup_context(c);
    }
  }
}


void *context_run(Context *ctx) {
  for (int i = 0; i < arrlen(ctx->statements); i++) {
    void *result = statement_run(ctx->statements[i], ctx);
    if (result != NULL)
      return result;
  }
  return NULL;
}


void context_destroy(Context *ctx) {
  for (int i = 0; i < arrlen(ctx->statements); i++)
    statement_destroy(ctx->statements[i]);
  for (int i = 0; i < arrlen(ctx->variables); i++)
    variable_destroy(ctx->variables[i]);
  for (int i = 0; i < arrlen(ctx->functions); i++)
    function_destroy(ctx->functions[i]);
  arrfree(ctx->statements);
  arrfree(ctx->variables);
  arrfree(ctx->functions);
  free(ctx);
}
