#include "vm.h"

Context *context_create(Context *parent) {
  Context *result = malloc(sizeof(Context));

  result->parent = parent;

  result->statements = NULL;
  result->variables = NULL;
  result->functions = NULL;
  // result->structs = NULL;

  return result;
}


Variable *context_variable_get(Context *ctx, const char *name) {
  for (int i = 0; i < arrlen(ctx->variables); i++) {
    Variable *v = ctx->variables[i];
    if (strcmp(v->name, name) == 0)
      return v;
  }
  if (ctx->parent != NULL)
    return context_variable_get(ctx->parent, name);
  return NULL;
}


void context_set_parents(Context *ctx) {
  for (int i = 0; i < arrlen(ctx->statements); i++) {
    Statement *stmt = ctx->statements[i];

    Context *c;

    switch (stmt->type) {
    case STMT_CTX:   c = ((StatementCtx*)stmt)->ctx; break;
    case STMT_IF:    c = ((StatementIf*)stmt)->ctx; break;
    case STMT_WHILE: c = ((StatementWhile*)stmt)->ctx; break;
    case STMT_ELSE:  c = ((StatementElse*)stmt)->ctx; break;
    default: continue;
    }

    c->parent = ctx;
    context_set_parents(c);
  }
}


int context_get_statement_index(Context *ctx, Statement *stmt) {
  for (int i = 0; i < arrlen(ctx->statements); i++)
    if (ctx->statements[i] == stmt)
      return i;
  return -1;
}


Value *context_run(Context *ctx) {
  for (int i = 0; i < arrlen(ctx->statements); i++) {
    Value *result = statement_run(ctx->statements[i], ctx);
    if (result->type->type != TYPE_NONE)
      return result;
  }
  return NULL;
}


void context_destroy(Context *ctx) {
  for (int i = 0; i < arrlen(ctx->statements); i++)
    statement_destroy(ctx->statements[i]);
  for (int i = 0; i < arrlen(ctx->variables); i++)
    variable_destroy(ctx->variables[i]);
  for (int i = 0; i < arrlen(ctx->functions); i++)
    function_destroy(ctx->functions[i]);
  // for (int i = 0; i < arrlen(ctx->structs); i++)
  //   struct_destroy(ctx->structs[i]);
  arrfree(ctx->statements);
  arrfree(ctx->variables);
  arrfree(ctx->functions);
  // arrfree(ctx->structs);
  free(ctx);
}
