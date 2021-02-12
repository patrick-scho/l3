#include "parse.h"

#include <file/file.h>

#include "source.h"

void error(const char *msg) {
  fprintf(stderr, "%s\n", msg);
}

bool parse_is_whitespace(char c) {
  return
    c == ' '  ||
    c == '\t' ||
    c == '\n' ||
    c == '\r';
}


bool parse_is_word_char(char c) {
  return
    !parse_is_whitespace(c) &&
    c != '(' &&
    c != ')' &&
    c != ',' &&
    c != '{' &&
    c != '}';
}


void parse_skip_whitespace(Source *s) {
  char c = source_get(s);
  while (parse_is_whitespace(c)) {
    s->index++;
    c = source_get(s);
  }
}


void parse_expect(Source *s, const char *str) {
  parse_skip_whitespace(s);
  int length = strlen(str);
  for (int i = 0; i < length; i++) {
    if (source_get(s) == str[i])
      s->index++;
    else
      error("Expected");
  }
}


bool parse_this_is(Source *s, const char *str) {
  int offset = 0;
  while (parse_is_whitespace(source_get_at(s, offset)))
    offset++;

  int len = strlen(str);
  for (int i = 0; i < len; i++)
    if (source_get_at(s, offset + i) != str[i])
      return false;
      
  return
    (s->index + offset + len >= s->length) ||
    !parse_is_word_char(source_get_at(s, offset + len));
}


bool parse_this_is_number(Source *s) {
  int offset = 0;
  while (parse_is_whitespace(source_get_at(s, offset)))
    offset++;

  char c = source_get_at(s, offset);
  return c >= '0' && c <= '9';
}


bool parse_next_is(Source *s, const char *str) {
  int offset = 0;
  while (parse_is_whitespace(source_get_at(s, offset)))
    offset++;
  while (!parse_is_whitespace(source_get_at(s, offset)))
    offset++;
  while (parse_is_whitespace(source_get_at(s, offset)))
    offset++;

  int len = strlen(str);
  for (int i = 0; i < len; i++)
    if (source_get_at(s, offset + i) != str[i])
      return false;
      
  return
    (s->index + offset + len >= s->length) ||
    !parse_is_word_char(source_get_at(s, offset + len));
}


bool parse_this_is_in(Source *s, const char **strs) {
  for (int i = 0; strs[i] != NULL; i++)
    if (parse_this_is(s, strs[i]))
      return true;

  return false;
}


char *parse_word(Source *s) {
  parse_skip_whitespace(s);
  int length = 0;
  while (parse_is_word_char(source_get_at(s, length)))
    length++;
  char *result = (char*)malloc((sizeof *result) * length + 1);
  strncpy(result, s->str + s->index, length);
  result[length] = 0;
  s->index += length;
  return result;
}


int parse_number(Source *s) {
  parse_skip_whitespace(s);
  int result = 0;
  char c = source_get(s);
  while (c >= '0' && c <= '9') {
    result *= 10;
    result += (c - '0');
    s->index++;
    c = source_get(s);
  }
  return result;
}
