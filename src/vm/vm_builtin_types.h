#pragma once

#include "vm.h"

extern Type type_builtin_int;
extern Type type_builtin_bool;

static void type_builtin_init() {
  type_builtin_int.type = TYPE_BASIC;
  type_builtin_int.byte_width = 8;
  type_builtin_bool.type = TYPE_BASIC;
  type_builtin_bool.byte_width = 1;
}

static void type_builtin_deinit() {

}