#include <stdio.h>

#include <vm/vm.h>

#include <vm/vm_types.h>
#include <util/memory.h>
#include <util/error.h>

#define STB_DS_IMPLEMENTATION
#include <stb_ds.h>

int main(int argc, char **argv) {
  Function *main;

  init_types();
  
  if (setjmp(error_jmp_buf) == 0) {
    main = mem_init(Function, NULL,
      .name = "main"
    );
    main->ctx = mem_init_cb(Context, main, context_free, .parent = NULL);


    Function *f1 = mem_init(Function, main,
      .name = "f1");
    f1->ctx = mem_init_cb(Context, f1, context_free, .parent = main->ctx);
    arrput(f1->parameters, mem_init(Variable, f1,
      .name = "a",
      .value = mem_init(Value, f1,
        .type = &type_int)));
    arrput(f1->parameters, mem_init(Variable, f1,
      .name = "b",
      .value = mem_init(Value, f1,
        .type = &type_int)));
    arrput(main->ctx->functions, f1);



    Expression *f1_call = mem_init(Expression, main,
      .type = EXPR_FUNC_CALL,
      .func_call.name = "f1");

    arrput(f1_call->func_call.params, mem_init(Expression, f1,
      .type = EXPR_LITERAL,
      .literal.value = mem_init(Value, f1,
        .type = &type_int,
        .value = 123)));
    arrput(f1_call->func_call.params, mem_init(Expression, f1,
      .type = EXPR_LITERAL,
      .literal.value = mem_init(Value, f1,
        .type = &type_int,
        .value = 12300)));


    arrput(main->ctx->statements, mem_init(Statement, main,
      .type = STMT_RETURN,
      .expr.expr = f1_call));

      
    arrput(f1->ctx->statements, mem_init(Statement, f1,
      .type = STMT_RETURN,
      .expr.expr = mem_init(Expression, f1,
        .type = EXPR_BINARY,
        .binary.op = BINARY_ADD,
        .binary.expr1 = mem_init(Expression, f1,
          .type = EXPR_VAR_GET,
          .var_get.name = "a"),
        .binary.expr2 = mem_init(Expression, f1,
          .type = EXPR_VAR_GET,
          .var_get.name = "b"))));

    Value *result = context_run(main->ctx);

    //if (result.type == type_int)
      printf("result: %d\n", (int)result->value);
  }
  
  mem_free(main);
  

  return 0;
}