#pragma once

#include <stdio.h>
#include <stdlib.h>

long file_read(const char *filename, char **buffer) {
  FILE *f = fopen(filename, "rb");

  fseek(f, 0, SEEK_END);
  long filesize = ftell(f);
  fseek(f, 0, SEEK_SET);

  *buffer = (char*)malloc((sizeof *buffer) * filesize);
  long read = fread(*buffer, 1, filesize, f);

  if (read != filesize)
    fprintf(stderr, "Unable to read whole file\n");

  fclose(f);

  return filesize;
}