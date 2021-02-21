#include "vm.h"

Value value_create(Type *type, void *value) {
  Value result;

  result.type = type;
  result.value = value;

  return result;
}
