#include "error.h"

jmp_buf error_jmp_buf;

void error(const char *msg) {
  fprintf(stderr, "Error: %s\n", msg);
  longjmp(error_jmp_buf, 1);
}
