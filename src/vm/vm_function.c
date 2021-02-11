#include "vm.h"

Function *function_create(const char *name, Context *ctx) {
  Function *result = malloc(sizeof *result);

  result->name = name;
  result->ctx = ctx;

  return result;
}


void *function_run(Function *f) {
  return context_run(f->ctx);
}


void function_destroy(Function *f) {
  context_destroy(f->ctx);
  free(f->name);
  free(f);
}
