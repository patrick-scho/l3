#pragma once

#include <stdbool.h>

#include <stb_ds.h>

#include <vm/vm.h>
#include "source.h"

void error(const char *msg);


bool  parse_is_whitespace(char c);
bool  parse_is_word_char(char c);
bool  parse_is_op_char(char c);
void  parse_skip_whitespace(Source *s);
void  parse_expect(Source *s, const char *str);
bool  parse_this_is(Source *s, const char *str);
bool  parse_this_is_number(Source *s);
bool  parse_next_is(Source *s, const char *str);
bool  parse_this_is_in(Source *s, const char **strs);
char *parse_word(Source *s);
char *parse_op(Source *s);
int   parse_number(Source *s);


ExpressionType op_to_expr_type(const char *op);

Variable   *parse_variable(Source *s);
Function   *parse_function(Source *s);
Expression *parse_expression(Source *s);
Statement  *parse_statement(Source *s);
Context    *parse_context(Source *s);