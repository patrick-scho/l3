#include "vm.h"

#include <util/error.h>

Value *value_unary(UnaryOp op, Value *value) {
  return value->type->unary_operators[op](value);
}

Value *value_binary(BinaryOp op, Value *value1, Value *value2) {
  if (!type_match(value1->type, value2->type))
    error("type mismatch");
  return value1->type->binary_operators[op](value1, value2);
}