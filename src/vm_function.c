#pragma once

#include "vm.h"

Function *function_create(const char *name) {
  Function *result = malloc(sizeof *result);

  result->ctx = context_create();

  return result;
}


void *function_run(Function *f) {
  return context_run(f->ctx);
}


void function_destroy(Function *f) {
  context_destroy(f->ctx);
  free(f);
}