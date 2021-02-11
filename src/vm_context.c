#pragma once

#include "vm.h"

Context *context_create(Context *parent, Statement **stmts) {
  Context *result = malloc(sizeof *result);

  result->parent = parent;

  result->statements = NULL;
  if (stmts != NULL)
    for (int i = 0; stmts[i] != NULL; i++)
      arrput(result->statements, stmts[i]);

  result->variables = NULL;

  return result;
}


void context_statement_add(Context *ctx, Statement *stmt) {
  arrput(ctx->statements, stmt);
}


void context_variable_add(Context *ctx, Variable *var) {
  arrput(ctx->variables, var);
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


void *context_run(Context *ctx) {
  for (int i = 0; i < arrlen(ctx->statements); i++) {
    void *result = statement_run(ctx, ctx->statements[i]);
    if (result != NULL)
      return result;
  }
  return NULL;
}


void context_destroy(Context *ctx) {
  for (int i = 0; i < arrlen(ctx->statements); i++) {
    statement_destroy(ctx->statements[i]);
  }
  for (int i = 0; i < arrlen(ctx->variables); i++) {
    variable_destroy(ctx->variables[i]);
  }
  arrfree(ctx->statements);
  free(ctx);
}