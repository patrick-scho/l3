#pragma once

#include <string.h>

#include <stb_ds.h>

#include "source.h"

const static char *keywords[] = {
  "var", "fn", "if", "while", "return"
};

const static char *TokenTypeNames[] = {
  "TOKEN_NAME", "TOKEN_OPERATOR", "TOKEN_STRING", "TOKEN_NUMBER",
  "TOKEN_PAR_OPEN", "TOKEN_PAR_CLOSE", "TOKEN_BRACE_OPEN", "TOKEN_BRACE_CLOSE",
  "TOKEN_BRACKET_OPEN", "TOKEN_BRACKET_CLOSE", "TOKEN_COMMA", "TOKEN_KEYWORD",
};

typedef enum {
  TOKEN_NAME, TOKEN_OPERATOR, TOKEN_STRING, TOKEN_NUMBER,
  TOKEN_PAR_OPEN, TOKEN_PAR_CLOSE, TOKEN_BRACE_OPEN, TOKEN_BRACE_CLOSE,
  TOKEN_BRACKET_OPEN, TOKEN_BRACKET_CLOSE, TOKEN_COMMA, TOKEN_KEYWORD
} TokenType;

typedef struct {
  TokenType type;
  char *str;
} Token;


bool is_whitespace(char c) {
  return
    c == ' '  ||
    c == '\t' ||
    c == '\n' ||
    c == '\r';
}
void skip_whitespace(Source *s) {
  while (is_whitespace(source_get(s)))
    s->index++;
}
void skip_comment(Source *s) {
  s->index += 2;
  int depth = 1;
  while (depth > 0) {
    if (source_get(s) == '*' && source_get_at(s, 1) == '/')
      depth--;
    if (source_get(s) == '/' && source_get_at(s, 1) == '*')
      depth++;

    s->index++;
  }
  s->index++;
}
bool is_numeric(char c) {
  return c >= '0' && c <= '9';
}
bool is_alpha(char c) {
  return
    (c >= 'a' && c <= 'z') ||
    (c >= 'A' && c <= 'Z') ||
    (c >= '0' && c <= '9') ||
    c == '_';
}
bool is_operator(char c) {
  const char ops[] = "+-*/=!<>|";
  for (int i = 0; i < sizeof(ops); i++)
    if (c == ops[i])
      return true;
  return false;
}


Token *token_create(TokenType type, char *str) {
  Token *result = (Token*)malloc(sizeof(Token));
  result->type = type;
  result->str = str;
}


Token *token_create_string(Source *s) {
  s->index++;
  int index_start = s->index;
  while (source_get(s) != '"' || source_get_at(s, -1) == '\\')
    s->index++;
  int index_end = s->index-1;
  int length = index_end - index_start;
  char *str = (char*)malloc(sizeof(char)*length+1);
  strncpy(str, s->str+index_start, length);
  str[length] = 0;
  return token_create(TOKEN_STRING, str);
}


Token *token_create_number(Source *s) {
  int index_start = s->index;
  while (is_numeric(source_get(s)))
    s->index++;
  int index_end = s->index;
  s->index--;
  int length = index_end - index_start;
  char *str = (char*)malloc(sizeof(char)*length+1);
  strncpy(str, s->str+index_start, length);
  str[length] = 0;
  return token_create(TOKEN_NUMBER, str);
}


Token *token_create_word(Source *s) {
  int index_start = s->index;
  while (is_alpha(source_get(s)))
    s->index++;
  int index_end = s->index;
  s->index--;
  int length = index_end - index_start;
  char *str = (char*)malloc(sizeof(char)*length+1);
  strncpy(str, s->str+index_start, length);
  str[length] = 0;
  TokenType type = TOKEN_NAME;
  int a = sizeof(keywords) / sizeof(char*);
  for (int i = 0; i < a; i++) {
    if (strcmp(keywords[i], str) == 0) {
      type = TOKEN_KEYWORD;
      break;
    }
  }
  return token_create(type, str);
}


Token *token_create_operator(Source *s) {
  int index_start = s->index;
  while (is_operator(source_get(s)))
    s->index++;
  int index_end = s->index;
  s->index--;
  int length = index_end - index_start;
  char *str = (char*)malloc(sizeof(char)*length+1);
  strncpy(str, s->str+index_start, length);
  str[length] = 0;
  return token_create(TOKEN_OPERATOR, str);
}


Token **lex_tokenize(Source *s) {
  Token **result = NULL;

  while (s->index < s->length) {
    while (is_whitespace(source_get(s)) ||
           (source_get(s) == '/' && source_get_at(s, 1) == '*')) {
      if (is_whitespace(source_get(s)))
        skip_whitespace(s);
      else
        skip_comment(s);
    }

    if (s->index >= s->length)
      break;

    char c = source_get(s);

    /**/ if (c == '"')      arrput(result, token_create_string(s));
    else if (c == '(')      arrput(result, token_create(TOKEN_PAR_OPEN,      strdup("(")));
    else if (c == ')')      arrput(result, token_create(TOKEN_PAR_CLOSE,     strdup(")")));
    else if (c == '{')      arrput(result, token_create(TOKEN_BRACE_OPEN,    strdup("{")));
    else if (c == '}')      arrput(result, token_create(TOKEN_BRACE_CLOSE,   strdup("}")));
    else if (c == '[')      arrput(result, token_create(TOKEN_BRACKET_OPEN,  strdup("[")));
    else if (c == ']')      arrput(result, token_create(TOKEN_BRACKET_CLOSE, strdup("]")));
    else if (c == ',')      arrput(result, token_create(TOKEN_COMMA,         strdup(",")));
    else if (is_numeric(c)) arrput(result, token_create_number(s));
    else if (is_alpha(c))   arrput(result, token_create_word(s));
    else                    arrput(result, token_create_operator(s));

    s->index++;
  }

  return result;
}


void token_destroy(Token *token) {
  free(token->str);
  free(token);
}


void token_stream_destroy(Token **tokens) {
  for (int i = 0; i < arrlen(tokens); i++) {
    token_destroy(tokens[i]);
  }
  arrfree(tokens);
}