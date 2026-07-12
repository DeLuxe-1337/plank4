#pragma once
#include <stdio.h>
#ifndef ARENA_H
#define ARENA_H

#include "alignment.h"
#include <assert.h>
#include <stdalign.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct ArenaBlock ArenaBlock;

struct ArenaBlock {
  ArenaBlock *next;

  size_t used;
  size_t size;

  unsigned char data[];
};

typedef struct {
  ArenaBlock *head;
  size_t default_block_size;
} Arena;

static inline size_t arena_align_up(size_t n, size_t align) {
  return (n + align - 1) & ~(align - 1);
}

static inline ArenaBlock *arena_new_block(size_t size) {
  printf("arena_new_block size=%zu\n", size);
  ArenaBlock *block = (ArenaBlock *)malloc(sizeof(*block) + size);

  assert(block);

  block->next = NULL;
  block->used = 0;
  block->size = size;

  return block;
}

static inline void arena_init(Arena *arena, size_t block_size) {
  arena->head = NULL;
  arena->default_block_size = block_size;
}

static inline void *arena_alloc(Arena *arena, size_t size) {
  size = arena_align_up(size, ARENA_ALIGNMENT);

  ArenaBlock *block = arena->head;

  if (!block || block->used + size > block->size) {

    size_t block_size = arena->default_block_size;

    if (size > block_size)
      block_size = size;

    ArenaBlock *new_block = arena_new_block(block_size);

    new_block->next = block;
    arena->head = new_block;

    block = new_block;

    printf("arena;\nblock=%p\n", (void *)block);
    printf("data=%p\n", (void *)block->data);
    printf("used=%zu\n", block->used);
    printf("size=%zu\n", block->size);
    printf("alloc=%zu\nend_arena;\n", size);
  }

  void *ptr = block->data + block->used;
  block->used += size;

  return ptr;
}

static inline void *arena_calloc(Arena *arena, size_t count, size_t size) {
  void *ptr = arena_alloc(arena, count * size);
  memset(ptr, 0, count * size);
  return ptr;
}

static inline char *arena_strdup(Arena *arena, const char *str) {
  size_t len = strlen(str) + 1;

  char *copy = (char *)arena_alloc(arena, len);

  memcpy(copy, str, len);

  return copy;
}

static inline void arena_reset(Arena *arena) {
  for (ArenaBlock *b = arena->head; b; b = b->next)
    b->used = 0;
}

static inline void arena_destroy(Arena *arena) {
  ArenaBlock *block = arena->head;

  while (block) {

    ArenaBlock *next = block->next;

    free(block);

    block = next;
  }

  arena->head = NULL;
}

#define arena_new(arena, Type) ((Type *)arena_alloc((arena), sizeof(Type)))

#define arena_new_zero(arena, Type)                                            \
  ((Type *)arena_calloc((arena), 1, sizeof(Type)))

#define arena_new_array(arena, Type, count)                                    \
  ((Type *)arena_alloc((arena), sizeof(Type) * (count)))

#define arena_new_array_zero(arena, Type, count)                               \
  ((Type *)arena_calloc((arena), (count), sizeof(Type)))

#endif