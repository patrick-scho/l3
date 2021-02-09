#pragma once

#include "vm.h"

Context *context_create() {
  Context *result = malloc(sizeof *result);

  vec_init(&result->statements);

  return result;
}


void context_statement_add(Context *ctx, Statement *stmt) {
  vec_push(&ctx->statements, stmt);
}


void *context_run(Context *ctx) {
  Statement *stmt; int i;
  vec_foreach (&ctx->statements, stmt, i) {
    void *result = statement_run(stmt);
    if (result != NULL)
      return result;
  }
  return NULL;
}


void context_destroy(Context *ctx) {
  Statement *stmt; int i;
  vec_foreach(&ctx->statements, stmt, i) {
    statement_destroy(stmt);
  }
  vec_deinit(&ctx->statements);
  free(ctx);
}