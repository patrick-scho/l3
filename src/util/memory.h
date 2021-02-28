#pragma once

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


void *mem_alloc_cb_(size_t size, void *parent, void (*cb)(void*), void *value);
void mem_free(void *ptr);