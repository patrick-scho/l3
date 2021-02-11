#include <stdio.h>

#include "vm.h"

#define STB_DS_IMPLEMENTATION
#include <stb_ds.h>

/*

TODO:

- Types Context
- return Value struct
- parameters
- FUNC_CALL
- C_CALL

*/

int main(int argc, char **argv) {
  Function *f1 = function_create(NULL, "f1");

  context_variable_add(f1->ctx, variable_create("a", 0));
  context_variable_add(f1->ctx, variable_create("b", 0));
  context_variable_add(f1->ctx, variable_create("c", 0));
  context_variable_add(f1->ctx, variable_create("n", 46));
  context_variable_add(f1->ctx, variable_create("i", 3));

  context_statement_add(f1->ctx,
    statement_create(STMT_IF, (void*[]){
      expression_create(EXPR_OR, (void*[]){
        expression_create(EXPR_EQUALS, (void*[]){
          expression_create(EXPR_VAR_GET, "n"),
          expression_create(EXPR_INT_LITERAL, 1)
        }),
        expression_create(EXPR_EQUALS, (void*[]){
          expression_create(EXPR_VAR_GET, "n"),
          expression_create(EXPR_INT_LITERAL, 2)
        })
      }),
      context_create(NULL, (Statement*[]){
        statement_create(STMT_RETURN, expression_create(EXPR_INT_LITERAL, 1)),
        NULL
      })
    })
  );
  context_statement_add(f1->ctx,
    statement_create(STMT_VAR_SET, (void*[]){ "a", expression_create(EXPR_INT_LITERAL, 1) })
  );
  context_statement_add(f1->ctx,
    statement_create(STMT_VAR_SET, (void*[]){ "b", expression_create(EXPR_INT_LITERAL, 1) })
  );
  
  context_statement_add(f1->ctx,
    statement_create(STMT_WHILE, (void*[]){
      expression_create(EXPR_LT, (void*[]){
        expression_create(EXPR_VAR_GET, "i"),
        expression_create(EXPR_VAR_GET, "n")
      }),
      context_create(f1->ctx, (Statement*[]){
        statement_create(STMT_VAR_SET, (void*[]){
          "c", expression_create(EXPR_VAR_GET, "a")
        }),
        statement_create(STMT_VAR_SET, (void*[]){
          "a", expression_create(EXPR_VAR_GET, "b")
        }),
        statement_create(STMT_VAR_SET, (void*[]){
          "b", expression_create(EXPR_ADD, (void*[]){
            expression_create(EXPR_VAR_GET, "b"),
            expression_create(EXPR_VAR_GET, "c")
          })
        }),
        statement_create(STMT_VAR_SET, (void*[]){
          "i", expression_create(EXPR_ADD, (void*[]){
            expression_create(EXPR_VAR_GET, "i"),
            expression_create(EXPR_INT_LITERAL, 1)
          })
        }),
        NULL
      })
    })
  );

  context_statement_add(f1->ctx,
    statement_create(STMT_RETURN,
      expression_create(EXPR_ADD, (void*[]){
        expression_create(EXPR_VAR_GET, "a"),
        expression_create(EXPR_VAR_GET, "b")
      })
    )
  );

  int result = (int)function_run(f1);

  printf("result: %d\n", result);

  function_destroy(f1);

  return 0;
}