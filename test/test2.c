#include <stdio.h>

#include <vm/vm.h>

#define STB_DS_IMPLEMENTATION
#include <stb_ds.h>

int main(int argc, char **argv) {
  if (setjmp(error_jmp_buf))
    goto cleanup;

  type_builtin_init();

  Function *f1 = function_create(strdup("f1"), context_create(NULL));

  arrput(f1->ctx->variables,
    variable_create(
      strdup("i1"),
      &type_builtin_int,
      value_create(&type_builtin_int, alloc_s64(12))));


  arrput(f1->ctx->statements,
    statement_create_var_set(
      strdup("i1"),
      expression_create_literal(EXPR_LITERAL_INT, (void*)13)));

  Context *ctx_if = context_create(f1->ctx);
  arrput(ctx_if->statements,
    statement_create_return(
      expression_create_var_get(strdup("i1"))));

  Context *ctx_else = context_create(f1->ctx);
  arrput(ctx_else->statements,
    statement_create_return(
      expression_create_literal(EXPR_LITERAL_INT, (void*)213)));

  arrput(f1->ctx->statements,
    statement_create_if(
      expression_create_binary(EXPR_BINARY_EQ,
        expression_create_var_get(strdup("i1")),
        expression_create_literal(EXPR_LITERAL_INT, (void*)13)),
      ctx_if));
  arrput(f1->ctx->statements,
    statement_create_else(ctx_else));

  Value *result = function_run(f1);

  //if (result.type == type_int)
    printf("result: %d\n", *(s64*)result->value);

cleanup:
  function_destroy(f1);

  return 0;
}