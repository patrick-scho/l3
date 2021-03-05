#include <stdio.h>

#include <vm/vm.h>

#include <vm/vm_types.h>
#include <util/memory.h>
#include <util/error.h>

#define STB_DS_IMPLEMENTATION
#include <stb_ds.h>

int main(int argc, char **argv) {
  Function *f1;

  init_types();
  
  if (setjmp(error_jmp_buf) == 0) {
    f1 = mem_init(Function, NULL,
      .name = "f1"
    );
    f1->ctx = mem_init_cb(Context, f1, context_free, .parent = NULL);


    arrput(f1->ctx->variables,
      mem_init(Variable, f1,
        .name = "i1",
        .value = mem_init(Value, f1,
          .type = &type_int,
          .value = 13)));

    Context *ctx_if = mem_init_cb(Context, f1, context_free, .parent = f1->ctx);
    arrput(ctx_if->statements,
      mem_init(Statement, f1,
        .type = STMT_RETURN,
        .expr.expr = mem_init(Expression, f1,
          .type = EXPR_LITERAL,
          .literal.value = mem_init(Value, f1, .type = &type_int, .value = 54321))));

    Context *ctx_else = mem_init_cb(Context, f1, context_free, .parent = f1->ctx);
    arrput(ctx_else->statements,
      mem_init(Statement, f1,
        .type = STMT_RETURN,
        .expr.expr = mem_init(Expression, f1,
          .type = EXPR_LITERAL,
          .literal.value = mem_init(Value, f1, .type = &type_int, .value = 12345))));

    arrput(f1->ctx->statements,
      mem_init(Statement, f1,
        .type = STMT_IF,
        .expr_ctx.expr = mem_init(Expression, f1,
          .type = EXPR_BINARY,
          .binary.op = BINARY_EQ,
          .binary.expr1 = mem_init(Expression, f1,
            .type = EXPR_VAR_GET,
            .var_get.name = "i1"),
          .binary.expr2 = mem_init(Expression, f1,
            .type = EXPR_LITERAL,
            .literal.value = mem_init(Value, f1, .type = &type_int, .value = 12))),
        .expr_ctx.ctx = ctx_if));

    arrput(f1->ctx->statements,
      mem_init(Statement, f1,
        .type = STMT_ELSE,
        .ctx.ctx = ctx_else));

    Value *result = context_run(f1->ctx);

    //if (result.type == type_int)
      printf("result: %d\n", (int)result->value);
  }
  
  mem_free(f1);
  

  return 0;
}