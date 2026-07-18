#pragma once

#include "arena.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  void *data;
  size_t len;
  size_t cap;
  size_t elem_size;
  Arena arena;
} Vector;

static inline Vector vector_create(size_t elem_size) {
  Vector v = (Vector){.elem_size = elem_size};
  arena_init(&v.arena, elem_size);
  return v;
}

static inline void vector_free(Vector *v) {
  arena_destroy(&v->arena);

  v->data = NULL;
  v->len = 0;
  v->cap = 0;
  v->elem_size = 0;
}

static inline void vector_clear(Vector *v) {
  arena_reset(&v->arena);

  v->data = NULL;
  v->len = 0;
  v->cap = 0;
}

static inline void vector_reserve(Vector *v, size_t cap) {
  if (cap <= v->cap)
    return;

  void *new_data = arena_alloc(&v->arena, cap * v->elem_size);

  if (v->data)
    memcpy(new_data, v->data, v->len * v->elem_size);

  v->data = new_data;
  v->cap = cap;
}

static inline void vector_resize(Vector *v, size_t len) {
  if (len > v->cap) {
    size_t cap = v->cap ? v->cap : 8;

    while (cap < len)
      cap *= 2;

    vector_reserve(v, cap);
  }

  if (len > v->len) {
    memset((char *)v->data + v->len * v->elem_size, 0,
           (len - v->len) * v->elem_size);
  }

  v->len = len;
}

static inline void vector_push(Vector *v, const void *elem) {
  if (v->len == v->cap)
    vector_reserve(v, v->cap ? v->cap * 2 : 8);

  memcpy((char *)v->data + v->len * v->elem_size, elem, v->elem_size);

  v->len++;
}

static inline void vector_pop(Vector *v, void *out) {
  assert(v->len);

  v->len--;

  if (out)
    memcpy(out, (char *)v->data + v->len * v->elem_size, v->elem_size);
}

static inline void *vector_at(Vector *v, size_t index) {
  assert(index < v->len);
  return (char *)v->data + index * v->elem_size;
}

static inline void *vector_back(Vector *v) {
  assert(v->len);
  return (char *)v->data + (v->len - 1) * v->elem_size;
}

static inline void *vector_begin(Vector *v) { return v->data; }

static inline void *vector_end(Vector *v) {
  return (char *)v->data + v->len * v->elem_size;
}

#define VECTOR(type) Vector

#define vector_init(type) vector_create(sizeof(type))

#define vector_push_value(v, value)                                            \
  do {                                                                         \
    __typeof__(value) _tmp = (value);                                          \
    vector_push((v), &_tmp);                                                   \
  } while (0)

#define vector_get(type, v, i) (*(type *)vector_at((v), (i)))

#define vector_back_value(type, v) (*(type *)vector_back((v)))
