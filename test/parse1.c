#include <stdio.h>

#include <vm/vm.h>
#include <parse/parse.h>

#define STB_DS_IMPLEMENTATION
#include <stb_ds.h>

int main(int argc, char **argv) {
  Source *source = source_create_from_file("test/a.txt");

  Context *ctx = parse_context(source);

  setup_context(ctx);

  int result = (int)context_run(ctx);
  printf("result: %d\n", result);

  source_destroy(source);
  context_destroy(ctx);

  return 0;
}