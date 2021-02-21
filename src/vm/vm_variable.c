#include "vm.h"

Variable *variable_create(char *name, Type *type, Value *value) {
  Variable *result = malloc(sizeof(Variable));

  result->name = name;
  result->type = type;
  result->value = value;
  
  return result;
}


void variable_destroy(Variable *var) {
  free(var->name);
  value_destroy(var->value);
  free(var);
}
