#include "vm.h"

#include "util/error.h"



Value *function_run(Function *f, Expression **parameters) {
  if (arrlen(f->parameters) != arrlen(parameters))
    error("Wrong number of parameters");

  for (int i = 0; i < arrlen(parameters); i++) {
    Variable *var = f->parameters[i];
    Expression *expr = parameters[i];

    Value *result = expression_run(expr, f->ctx);

    // TODO: type match
    // if (result->type != var->value->type)

    context_variable_set(f->ctx, var->name, result);
  }

  return context_run(f->ctx);
}