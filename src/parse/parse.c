#include "parse.h"

#include <vm/vm.h>
#include <file/file.h>

#include "source.h"

static const char op_chars[] = "+-*/&|!=<>";

bool parse_is_whitespace(char c) {
  return
    c == ' '  ||
    c == '\t' ||
    c == '\n' ||
    c == '\r';
}


bool parse_is_word_char(char c) {
  return
    (c >= 'a' && c <= 'z') ||
    (c >= 'A' && c <= 'Z') ||
    (c >= '0' && c <= '9') ||
    c == '_';
}


bool parse_is_op_char(char c) {
  for (size_t i = 0; i < strlen(op_chars); i++) {
    if (c == op_chars[i])
      return true;
  }
  return false;
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
      
  return true;
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
      
  return true;
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
  char *result = (char*)malloc(sizeof(char) * length + 1);
  strncpy(result, s->str + s->index, length);
  result[length] = 0;
  s->index += length;
  return result;
}


char *parse_op(Source *s) {
  parse_skip_whitespace(s);
  int length = 0;
  while (parse_is_op_char(source_get_at(s, length)))
    length++;
  char *result = (char*)malloc(sizeof(char) * length + 1);
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
