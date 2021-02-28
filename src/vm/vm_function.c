#include "vm.h"



Value *function_run(Function *f) {
  return context_run(f->ctx);
}

