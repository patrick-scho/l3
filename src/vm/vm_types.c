#include "vm_types.h"

// INT

Type type_int;


#define INT_BINARY_OP(name, return_type, op) Value *type_int_##name(Value *v1, Value *v2) { return mem_init(Value, v1, .type = &return_type, .value = (int)v1->value op (int)v2->value); }
INT_BINARY_OP(add, type_int, +)
INT_BINARY_OP(sub, type_int, -)
INT_BINARY_OP(mul, type_int, *)
INT_BINARY_OP(div, type_int, /)
INT_BINARY_OP(eq, type_bool, ==)
INT_BINARY_OP(not_eq, type_bool, !=)
INT_BINARY_OP(lt, type_bool, <)
INT_BINARY_OP(gt, type_bool, >)
INT_BINARY_OP(lt_eq, type_bool, <=)
INT_BINARY_OP(gt_eq, type_bool, >=)
#undef INT_BINARY_OP

// BOOL

Type type_bool;

#define BOOL_UNARY_OP(name, return_type, op) Value *type_bool_##name(Value *v1) { return mem_init(Value, v1, .type = &return_type, .value = (void*) op (bool)v1->value); }
BOOL_UNARY_OP(not, type_bool, !)
#undef BOOL_UNARY_OP

#define BOOL_BINARY_OP(name, return_type, op) Value *type_bool_##name(Value *v1, Value *v2) { return mem_init(Value, v1, .type = &return_type, .value = (bool)v1->value op (bool)v2->value); }
BOOL_BINARY_OP(eq, type_bool, ==)
BOOL_BINARY_OP(not_eq, type_bool, !=)
BOOL_BINARY_OP(lt, type_bool, <)
BOOL_BINARY_OP(gt, type_bool, >)
BOOL_BINARY_OP(lt_eq, type_bool, <=)
BOOL_BINARY_OP(gt_eq, type_bool, >=)
#undef BOOL_BINARY_OP






Value *type_copy_value(Value *v) {
  Value *result = mem_init(Value, v,
    .type = v->type,
    .value = v->value);

  return result;
}



void init_types() {
  type_int = (Type) {
    .type = TYPE_BASIC,
    .name = "int",
    .byte_width = 4,
    .binary_operators = {
      [BINARY_ADD] = type_int_add,
      [BINARY_SUB] = type_int_sub,
      [BINARY_MUL] = type_int_mul,
      [BINARY_DIV] = type_int_div,
      [BINARY_EQ] = type_int_eq,
      [BINARY_NOT_EQ] = type_int_not_eq,
      [BINARY_LT] = type_int_lt,
      [BINARY_GT] = type_int_gt,
      [BINARY_LT_EQ] = type_int_lt_eq,
      [BINARY_GT_EQ] = type_int_gt_eq,
    },
    .copy = type_copy_value
  };
  type_bool = (Type) {
    .type = TYPE_BASIC,
    .name = "bool",
    .byte_width = 1,
    .unary_operators = {
      [UNARY_NOT] = type_bool_not
    },
    .binary_operators = {
      [BINARY_EQ] = type_bool_eq,
      [BINARY_NOT_EQ] = type_bool_not_eq,
      [BINARY_LT] = type_bool_lt,
      [BINARY_GT] = type_bool_gt,
      [BINARY_LT_EQ] = type_bool_lt_eq,
      [BINARY_GT_EQ] = type_bool_gt_eq,
    },
    .copy = type_copy_value
  };
}