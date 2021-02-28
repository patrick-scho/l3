#include <stdlib.h>

#include "stb_ds.h"

typedef struct Memory Memory;

typedef struct Memory {
  void *ptr;
  void (*cb)(void*);
  Memory *parent;
  Memory **children;
} Memory;

#define mem_alloc(type, parent) (type*)mem_alloc_cb_(sizeof(type), parent, NULL, NULL)
#define mem_alloc_cb(type, parent, cb) (type*)mem_alloc_cb_(sizeof(type), parent, (void(*)(void*))cb, NULL)

#define mem_init(type, parent, ...) (type*)mem_alloc_cb_(sizeof(type), parent, NULL, &(type) { __VA_ARGS__ })
#define mem_init_cb(type, parent, cb, ...) (type*)mem_alloc_cb_(sizeof(type), parent, (void(*)(void*))cb, &(type) { __VA_ARGS__ })


void *mem_alloc_cb_(size_t size, void *parent, void (*cb)(void*), void *value) {
  void *ptr = malloc(sizeof(Memory) + size);
  Memory *mem = (Memory*)ptr;
  ptr = (Memory*)ptr + 1;

  Memory *mem_parent = (parent == NULL) ? NULL : (Memory*)parent - 1;

  mem->ptr = ptr;
  mem->cb = cb;
  mem->parent = mem_parent;
  mem->children = NULL;

  if (mem->parent != NULL)
    arrput(mem->parent->children, mem);

  if (value != NULL)
    memcpy(mem->ptr, value, size);

  return ptr;
}

void mem_free(void *ptr) {
  Memory *mem = (Memory*)ptr - 1;

  for (int i = 0; i < arrlen(mem->children); i++)
    mem_free(mem->children[i]->ptr);

  arrfree(mem->children);

  if (mem->cb != NULL)
    mem->cb(ptr);

  free(mem);
}