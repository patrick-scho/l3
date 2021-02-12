#include <stdio.h>

#include <vm/vm.h>
#include <parse/source.h>
#include <parse/lex.h>
#include <parse/parse.h>

#define STB_DS_IMPLEMENTATION
#include <stb_ds.h>

int main(int argc, char **argv) {
  if (argc != 2) {
    printf("Wrong number of arguments\n");
    return 1;
  }
  Source *source = source_create_from_file(argv[1]);

  Token **tokens = lex_tokenize(source);
  
  for (int i = 0; i < arrlen(tokens); i++) {
    printf("%s (%s)\n", TokenTypeNames[tokens[i]->type], tokens[i]->str);
  }

  // Context *ctx = parse_context(source);

  // context_set_parents(ctx);

  // int result = (int)context_run(ctx);
  // printf("result: %d\n", result);

  token_stream_destroy(tokens);
  source_destroy(source);
  //context_destroy(ctx);

  return 0;
}