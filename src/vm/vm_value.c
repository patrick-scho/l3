#include "vm.h"

#include <util/memory.h>
#include <util/error.h>

Value *value_unary(UnaryOp op, Value *value) {
  return value->type->unary_operators[op](value);
}

Value *value_binary(BinaryOp op, Value *value1, Value *value2) {
  if (!type_match(value1->type, value2->type))
    error("type mismatch");
  return value1->type->binary_operators[op](value1, value2);
}

void value_set(Value *v1, Value *v2) {
  if (!type_match(v1->type, v2->type))
    error("type mismatch");
  
  memcpy(v1->value, v2->value, type_get_byte_width(v1->type));
}

Value *value_copy(Value *value) {
  Value *result = value->type->copy(value);

  return result;
}
