#include "source.h"

Source *source_create_from_file(const char *filename) {
  Source *result = (Source*)malloc(sizeof(Source));
  result->length = file_read(filename, &(result->str));
  result->index = 0;
  return result;
}


char source_get(Source *s) {
  if (s->index >= s->length)
    error("source_get index out of bounds");
  return s->str[s->index];
}


char source_get_at(Source *s, int offset) {
  if (s->index + offset >= s->length)
    error("source_get_at index out of bounds");
  return s->str[s->index + offset];
}


void source_destroy(Source *s) {
  free(s->str);
  free(s);
}