#include "vm.h"

bool type_match(Type *type1, Type *type2) {
  if (type1->type == type2->type)
    return true;
  return false;
}

// Type *type_create(TypeType type) {
//   Type *result = (Type*)malloc(sizeof(Type));

//   result->type = type;

//   return result;
// }


void *type_alloc(Type *type) {
  return malloc(type_get_byte_width(type));
}


uint type_get_byte_width(Type *type) {
  switch (type->type) {
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
}


// void type_destroy(Type *type) {
//   free(type);
// }
