#pragma once

#include "vm.h"

Variable *variable_create(const char *name, void *value) {
  Variable *result = malloc(sizeof *result);
  result->name = name;
  result->value = value;
  return result;
}


void variable_destroy(Variable *var) {
  free(var);
}