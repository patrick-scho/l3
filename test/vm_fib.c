#include <stdio.h>

#include <vm/vm.h>

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
  Function *f1 = function_create(strdup("f1"), context_create(NULL, NULL));

  context_variable_add(f1->ctx, variable_create(strdup("a"), 0));
  context_variable_add(f1->ctx, variable_create(strdup("b"), 0));
  context_variable_add(f1->ctx, variable_create(strdup("c"), 0));
  context_variable_add(f1->ctx, variable_create(strdup("n"), 46));
  context_variable_add(f1->ctx, variable_create(strdup("i"), 3));

  context_statement_add(f1->ctx,
    statement_create(STMT_IF,
      expression_create(EXPR_OR,
        expression_create(EXPR_EQUALS,
          expression_create(EXPR_VAR_GET, strdup("n"), NULL),
          expression_create(EXPR_INT_LITERAL, 1, NULL)
        ),
        expression_create(EXPR_EQUALS,
          expression_create(EXPR_VAR_GET, strdup("n"), NULL),
          expression_create(EXPR_INT_LITERAL, 2, NULL)
        )
      ),
      context_create(NULL, (Statement*[]){
        statement_create(STMT_RETURN, expression_create(EXPR_INT_LITERAL, 1, NULL), NULL),
        NULL
      })
    )
  );
  context_statement_add(f1->ctx,
    statement_create(STMT_VAR_SET, strdup("a"), expression_create(EXPR_INT_LITERAL, 1, NULL) )
  );
  context_statement_add(f1->ctx,
    statement_create(STMT_VAR_SET, strdup("b"), expression_create(EXPR_INT_LITERAL, 1, NULL) )
  );
  
  context_statement_add(f1->ctx,
    statement_create(STMT_WHILE,
      expression_create(EXPR_LT,
        expression_create(EXPR_VAR_GET, strdup("i"), NULL),
        expression_create(EXPR_VAR_GET, strdup("n"), NULL)
      ),
      context_create(f1->ctx, (Statement*[]){
        statement_create(STMT_VAR_SET,
          strdup("c"), expression_create(EXPR_VAR_GET, strdup("a"), NULL)
        ),
        statement_create(STMT_VAR_SET,
          strdup("a"), expression_create(EXPR_VAR_GET, strdup("b"), NULL)
        ),
        statement_create(STMT_VAR_SET,
          strdup("b"), expression_create(EXPR_ADD,
            expression_create(EXPR_VAR_GET, strdup("b"), NULL),
            expression_create(EXPR_VAR_GET, strdup("c"), NULL)
          )
        ),
        statement_create(STMT_VAR_SET,
          strdup("i"), expression_create(EXPR_ADD,
            expression_create(EXPR_VAR_GET, strdup("i"), NULL),
            expression_create(EXPR_INT_LITERAL, 1, NULL)
          )
        ),
        NULL
      })
    )
  );

  context_statement_add(f1->ctx,
    statement_create(STMT_RETURN,
      expression_create(EXPR_ADD,
        expression_create(EXPR_VAR_GET, strdup("a"), NULL),
        expression_create(EXPR_VAR_GET, strdup("b"), NULL)
      ), NULL
    )
  );

  int result = (int)function_run(f1);

  printf("result: %d\n", result);

  function_destroy(f1);

  return 0;
}