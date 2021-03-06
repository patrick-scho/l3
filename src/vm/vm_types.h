#pragma once

#include <vm/vm.h>

#include <util/memory.h>

// INT

extern Type type_int;

#define INT_BINARY_OP(name, return_type, op) Value *type_int_##name(Value *v1, Value *v2);
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

extern Type type_char;

#define CHAR_BINARY_OP(name, return_type, op) Value *type_char_##name(Value *v1, Value *v2);
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

extern Type type_float;

#define FLOAT_BINARY_OP(name, return_type, op) Value *type_float_##name(Value *v1, Value *v2);
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

extern Type type_bool;

#define BOOL_UNARY_OP(name, return_type, op) Value *type_bool_##name(Value *v1);
BOOL_UNARY_OP(not, type_bool, !)
#undef BOOL_UNARY_OP

#define BOOL_BINARY_OP(name, return_type, op) Value *type_bool_##name(Value *v1, Value *v2);
BOOL_BINARY_OP(eq, type_bool, ==)
BOOL_BINARY_OP(not_eq, type_bool, !=)
BOOL_BINARY_OP(lt, type_bool, <)
BOOL_BINARY_OP(gt, type_bool, >)
BOOL_BINARY_OP(lt_eq, type_bool, <=)
BOOL_BINARY_OP(gt_eq, type_bool, >=)
#undef BOOL_BINARY_OP




Value *type_copy_value(Value *v);


void init_types();