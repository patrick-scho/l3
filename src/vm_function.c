#pragma once

#include "vm.h"

Function *function_create(const char *name, Type returnType, Type *parameterTypes) {
  Function *result = malloc(sizeof *result);
  result->returnType = returnType;

  vec_init(&result->parameterTypes);
  for (int i = 0; parameterTypes[i].name != NULL; i++)
    vec_push(&result->parameterTypes, parameterTypes[i]);

  result->ctx = context_create();

  return result;
}


void *function_run(Function *f) {
  return context_run(f->ctx);
}


void function_destroy(Function *f) {
  context_destroy(f->ctx);
  vec_deinit(&f->parameterTypes);
  free(f);
}