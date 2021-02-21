#include <stdio.h>

#include <vm/vm.h>

#define STB_DS_IMPLEMENTATION
#include <stb_ds.h>

int main(int argc, char **argv) {
  Function *f1 = function_create(strdup("f1"), context_create(NULL));

  Type *type_int = type_create(TYPE_BASIC);
  type_int->byte_width = 4;



  Struct *struct_a = struct_create(strdup("A"));
  shput(struct_a->members, "i1", type_int);
  shput(struct_a->members, "i2", type_int);

  arrput(f1->ctx->structs, struct_a);

  Type *type_a = type_create(TYPE_STRUCT);
  type_a->type_struct = struct_a;




  Struct *struct_b = struct_create(strdup("B"));
  shput(struct_a->members, "a1", type_a);
  shput(struct_a->members, "a2", type_a);

  arrput(f1->ctx->structs, struct_b);

  Type *type_b = type_create(TYPE_STRUCT);
  type_b->type_struct = struct_b;



  arrput(f1->ctx->variables,
    variable_create(
      strdup("b1"),
      value_create(type_b, NULL)));
  arrput(f1->ctx->variables,
    variable_create(
      strdup("i1"),
      value_create(type_int, 12)));


  Context *ctx_if = context_create(f1->ctx);
  arrput(ctx_if->statements,
    statement_create(STMT_RETURN,
      expression_create(EXPR_STRUCT_MEMBER_GET,
        expression_create(EXPR_STRUCT_MEMBER_GET,
          expression_create(EXPR_VAR_GET, strdup("b1"), NULL),
          "a1"),
        "i1"),
      NULL));
  Context *ctx_else = context_create(f1->ctx);
  arrput(ctx_else->statements,
    statement_create(STMT_RETURN,
      expression_create(EXPR_STRUCT_MEMBER_GET,
        expression_create(EXPR_STRUCT_MEMBER_GET,
          expression_create(EXPR_VAR_GET, strdup("b1"), NULL),
          "a2"),
        "i2"),
      NULL));

  arrput(f1->ctx->statements,
    statement_create(STMT_IF,
      expression_create(EXPR_EQUALS,
        expression_create(EXPR_VAR_GET, strdup("i1"), NULL),
        expression_create(EXPR_INT_LITERAL, 13, NULL)),
      ctx_if));
  arrput(f1->ctx->statements,
    statement_create(STMT_ELSE,
      NULL,
      ctx_else));

  Value *result = function_run(f1);

  //if (result.type == type_int)
    printf("result: %d\n", (int)result->value);

  function_destroy(f1);

  return 0;
}