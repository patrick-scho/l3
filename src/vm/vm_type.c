#include "vm.h"

#include "util/memory.h"


void *type_alloc(Type *type, void *parent) {
  return mem_alloc_(type_get_byte_width(type), parent, NULL, NULL);
}


uint type_get_byte_width(Type *type) {
  switch (type->type) {
  case TYPE_NONE:
  case TYPE_STRUCT:
    break;
  case TYPE_BASIC:
    return type->byte_width;
  case TYPE_PTR:
    return sizeof(void*);
  case TYPE_ARRAY:
    return type_get_byte_width(type->array_type) * type->array_length;
  // case TYPE_STRUCT: {
  //   uint sum = 0;
  //   for (int i = 0; i < shlen(type->type_struct->members); i++)
  //     sum += type_get_byte_width(type->type_struct->members[i].value);
  //   return sum;
  // }
  }
  return 0;
}


bool type_match(Type *type1, Type *type2) {
  return (
    (type1->type == type2->type) &&
    (strcmp(type1->name, type2->name) == 0)
  );
}

