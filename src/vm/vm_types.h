#pragma once

#include <vm/vm.h>

#include <util/memory.h>

extern Type type_int, type_bool;

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

void init_types();