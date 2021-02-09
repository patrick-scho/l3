#include <stdio.h>

#include <vec.h>

#include "vm.h"

/*

TODO:

- Types/Variables Context
- return Value struct
- parameters
- more expr/stmt

*/

int main(int argc, char **argv) {
  Function *f1 = function_create("f1", TYPE_INT, (Type[]){ TYPE_INT, {NULL} });
  
  Context *ctx_if = context_create();
  context_statement_add(ctx_if,
    statement_create(STMT_RETURN,
      expression_create(EXPR_INT_LITERAL, (void*)123)
    )
  );

  context_statement_add(f1->ctx,
    statement_create(STMT_IF, (void*[]){
      expression_create(EXPR_EQUALS, (void*[]){
        expression_create(EXPR_INT_LITERAL, (void*)1),
        expression_create(EXPR_INT_LITERAL, (void*)2)
      }),
      ctx_if
    })
  );
  context_statement_add(f1->ctx,
    statement_create(STMT_RETURN,
      expression_create(EXPR_ADD, (void*[]){
        expression_create(EXPR_ADD, (void*[]){
          expression_create(EXPR_INT_LITERAL, (void*)12),
          expression_create(EXPR_INT_LITERAL, (void*)34)
        }),
        expression_create(EXPR_INT_LITERAL, (void*)34)
      })
    )
  );

  int result = (int)function_run(f1);

  printf("result: %d\n", result);

  function_destroy(f1);

  return 0;
}