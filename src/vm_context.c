#pragma once

#include "vm.h"

Context *context_create() {
  Context *result = malloc(sizeof *result);

  result->statements = NULL;

  return result;
}


void context_statement_add(Context *ctx, Statement *stmt) {
  arrput(ctx->statements, stmt);
}


void *context_run(Context *ctx) {
  for (int i = 0; i < arrlen(ctx->statements); i++) {
    void *result = statement_run(ctx->statements[i]);
    if (result != NULL)
      return result;
  }
  return NULL;
}


void context_destroy(Context *ctx) {
  for (int i = 0; i < arrlen(ctx->statements); i++) {
    statement_destroy(ctx->statements[i]);
  }
  arrfree(ctx->statements);
  free(ctx);
}