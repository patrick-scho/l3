#include <stdio.h>

#include "vm.h"

#define STB_DS_IMPLEMENTATION
#include <stb_ds.h>

/*

TODO:

- Types/Variables Context
- return Value struct
- parameters
- more expr/stmt
- STMT_CONTEXT

*/

int main(int argc, char **argv) {
  Function *f1 = function_create("f1");

  context_statement_add(f1->ctx,
    statement_create(STMT_IF, (void*[]){
      expression_create(EXPR_NOT, 
        expression_create(EXPR_LT, (void*[]){
          expression_create(EXPR_INT_LITERAL, (void*)101),
          expression_create(EXPR_INT_LITERAL, (void*)102)
        })
      ),
      context_create((Statement*[]){
        statement_create(STMT_RETURN,
          expression_create(EXPR_INT_LITERAL, (void*)123)
        ),
        NULL
      })
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