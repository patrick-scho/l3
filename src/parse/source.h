#pragma once

#include <stdlib.h>

#include <file/file.h>


typedef struct {
  char *str;
  unsigned int index, length;
} Source;

Source *source_create_from_file(const char *filename);
char source_get(Source *s);
char source_get_at(Source *s, int offset);
void source_destroy(Source *s);