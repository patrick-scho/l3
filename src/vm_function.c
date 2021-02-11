#pragma once

#include "vm.h"

Function *function_create(Context *ctx, const char *name) {
  Function *result = malloc(sizeof *result);

  result->name = name;
  result->ctx = context_create(ctx, NULL);

  return result;
}


void *function_run(Function *f) {
  return context_run(f->ctx);
}


void function_destroy(Function *f) {
  context_destroy(f->ctx);
  free(f);
}