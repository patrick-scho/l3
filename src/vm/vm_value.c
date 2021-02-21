#include "vm.h"

Value *value_create(Type *type, void *value) {
  Value *result = (Value*)malloc(sizeof(Value));

  result->type = type;
  result->value = value;

  return result;
}


void value_destroy(Value *value) {
  free(value->value);
  free(value);
}
