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

// CHAR

Type type_char;

#define CHAR_BINARY_OP(name, return_type, op) Value *type_char_##name(Value *v1, Value *v2) { return mem_init(Value, v1, .type = &return_type, .value = (char)v1->value op (char)v2->value); }
CHAR_BINARY_OP(add, type_char, +)
CHAR_BINARY_OP(sub, type_char, -)
CHAR_BINARY_OP(mul, type_char, *)
CHAR_BINARY_OP(div, type_char, /)
CHAR_BINARY_OP(eq, type_bool, ==)
CHAR_BINARY_OP(not_eq, type_bool, !=)
CHAR_BINARY_OP(lt, type_bool, <)
CHAR_BINARY_OP(gt, type_bool, >)
CHAR_BINARY_OP(lt_eq, type_bool, <=)
CHAR_BINARY_OP(gt_eq, type_bool, >=)
#undef CHAR_BINARY_OP

// FLOAT

Type type_float;

typedef union {
  float f; void *v;
} uf;

#define FLOAT_BINARY_OP(name, return_type, op) Value *type_float_##name(Value *v1, Value *v2) { \
uf u1, u2, u3; u1.v = v1->value; u2.v = v2->value; u3.f = u1.f op u2.f; \
return mem_init(Value, v1, .type = &return_type, .value = u3.v); }
FLOAT_BINARY_OP(add, type_float, +)
FLOAT_BINARY_OP(sub, type_float, -)
FLOAT_BINARY_OP(mul, type_float, *)
FLOAT_BINARY_OP(div, type_float, /)
FLOAT_BINARY_OP(eq, type_bool, ==)
FLOAT_BINARY_OP(not_eq, type_bool, !=)
FLOAT_BINARY_OP(lt, type_bool, <)
FLOAT_BINARY_OP(gt, type_bool, >)
FLOAT_BINARY_OP(lt_eq, type_bool, <=)
FLOAT_BINARY_OP(gt_eq, type_bool, >=)
#undef FLOAT_BINARY_OP

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
  type_char = (Type) {
    .type = TYPE_BASIC,
    .name = "char",
    .byte_width = 1,
    .binary_operators = {
      [BINARY_ADD] = type_char_add,
      [BINARY_SUB] = type_char_sub,
      [BINARY_MUL] = type_char_mul,
      [BINARY_DIV] = type_char_div,
      [BINARY_EQ] = type_char_eq,
      [BINARY_NOT_EQ] = type_char_not_eq,
      [BINARY_LT] = type_char_lt,
      [BINARY_GT] = type_char_gt,
      [BINARY_LT_EQ] = type_char_lt_eq,
      [BINARY_GT_EQ] = type_char_gt_eq,
    },
    .copy = type_copy_value
  };
  type_float = (Type) {
    .type = TYPE_BASIC,
    .name = "float",
    .byte_width = 4,
    .binary_operators = {
      [BINARY_ADD] = type_float_add,
      [BINARY_SUB] = type_float_sub,
      [BINARY_MUL] = type_float_mul,
      [BINARY_DIV] = type_float_div,
      [BINARY_EQ] = type_float_eq,
      [BINARY_NOT_EQ] = type_float_not_eq,
      [BINARY_LT] = type_float_lt,
      [BINARY_GT] = type_float_gt,
      [BINARY_LT_EQ] = type_float_lt_eq,
      [BINARY_GT_EQ] = type_float_gt_eq,
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