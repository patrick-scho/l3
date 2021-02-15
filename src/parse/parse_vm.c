#include "parse.h"

#include <vm/vm.h>
#include "source.h"

static const char *operators[] = { "+", "-", "*", "/", "==", "!=", "<", ">", "<=", ">=", "&&", "||", NULL };

ExpressionType op_to_expr_type(const char *op) {
  /**/ if (strcmp(op, "+")  == 0) return EXPR_ADD;
  else if (strcmp(op, "-")  == 0) return EXPR_SUB;
  else if (strcmp(op, "*")  == 0) return EXPR_MUL;
  else if (strcmp(op, "/")  == 0) return EXPR_DIV;
  else if (strcmp(op, "==") == 0) return EXPR_EQUALS;
  else if (strcmp(op, "!=") == 0) return EXPR_NOT_EQUALS;
  else if (strcmp(op, "<")  == 0) return EXPR_LT;
  else if (strcmp(op, ">")  == 0) return EXPR_GT;
  else if (strcmp(op, "<=") == 0) return EXPR_LT_EQ;
  else if (strcmp(op, ">=") == 0) return EXPR_GT_EQ;
  else if (strcmp(op, "&&") == 0) return EXPR_AND;
  else if (strcmp(op, "||") == 0) return EXPR_OR;
}


Variable *parse_variable(Source *s) {
  parse_expect(s, "var");
  char *name = parse_word(s);
  parse_expect(s, "=");
  int value = parse_number(s);
  return variable_create(name, value);
}


Function *parse_function(Source *s) {
  parse_expect(s, "fn");
  char *name = parse_word(s);
  parse_expect(s, "()");
  Context *ctx = parse_context(s);
  return function_create(name, ctx);
}


Expression *parse_subexpression(Source *s) {
  /**/ if (parse_this_is(s, "(")) {
    parse_expect(s, "(");
    Expression *expr = parse_expression(s);
    parse_expect(s, ")");
    return expr;
  }
  else if (parse_this_is(s, "!")) {
    parse_expect(s, "!");
    Expression *expr = parse_expression(s);
    return expression_create(EXPR_NOT, expr, NULL);
  }
  else if (parse_this_is_number(s)) {
    int number = parse_number(s);
    return expression_create(EXPR_INT_LITERAL, number, NULL);
  }
  else {
    char *name = parse_word(s);
    return expression_create(EXPR_VAR_GET, name, NULL);
  }
}


Expression *parse_expression(Source *s) {
  Expression *result = parse_subexpression(s);

  while (parse_this_is_in(s, operators)) {
    char *op = parse_op(s);
    
    Expression *exprL = result;
    Expression *exprR = parse_subexpression(s);
    
    ExpressionType type = op_to_expr_type(op);
    free(op);
    result = expression_create(type, exprL, exprR);
  }

  return result;
}


Statement *parse_statement(Source *s) {
  /**/ if (parse_this_is(s, "return")) {
    parse_expect(s, "return");
    Expression *expr = parse_expression(s);
    return statement_create(STMT_RETURN, expr, NULL);
  }
  else if (parse_this_is(s, "if")) {
    parse_expect(s, "if");
    Expression *expr = parse_expression(s);
    Context *ctx = parse_context(s);
    return statement_create(STMT_IF, expr, ctx);
  }
  else if (parse_this_is(s, "else")) {
    parse_expect(s, "else");
    Context *ctx = parse_context(s);
    return statement_create(STMT_ELSE, NULL, ctx);
  }
  else if (parse_this_is(s, "while")) {
    parse_expect(s, "while");
    Expression *expr = parse_expression(s);
    Context *ctx = parse_context(s);
    return statement_create(STMT_WHILE, expr, ctx);
  }
  else if (parse_this_is(s, "{")) {
    Context *ctx = parse_context(s);
    return statement_create(STMT_CTX, NULL, ctx);
  }
  else if (parse_next_is(s, "=")) {
    char *name = parse_word(s);
    parse_expect(s, "=");
    Expression *expr = parse_expression(s);
    return statement_create(STMT_VAR_SET, name, expr);
  }
  else {
    Expression *expr = parse_expression(s);
    return statement_create(STMT_EXPR, expr, NULL);
  }
}


Context *parse_context(Source *s) {
  Context *result = context_create(NULL, NULL);

  parse_expect(s, "{");

  while (! parse_this_is(s, "}")) {

    if (parse_this_is(s, "var")) {
      context_variable_add(result,
        parse_variable(s));
    }
    else if (parse_this_is(s, "fn")) {
      context_function_add(result,
        parse_function(s));
    }
    else {
      context_statement_add(result,
        parse_statement(s));
    }

  }

  parse_expect(s, "}");

  return result;
}
