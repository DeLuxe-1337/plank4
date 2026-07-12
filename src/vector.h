#pragma once
#ifndef VECTOR_H
#define VECTOR_H

#include <assert.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  void *data;
  size_t len;
  size_t cap;
  size_t elem_size;
} Vector;

static inline Vector vector_create(size_t elem_size) {
  return (Vector){.elem_size = elem_size};
}

static inline void vector_destroy(Vector *v) {
  free(v->data);

  v->data = NULL;
  v->len = 0;
  v->cap = 0;
}

static inline void vector_reserve(Vector *v, size_t cap) {
  if (cap <= v->cap)
    return;

  void *new_data = realloc(v->data, cap * v->elem_size);
  assert(new_data);

  v->data = new_data;
  v->cap = cap;
}

static inline void vector_push(Vector *v, const void *elem) {
  if (v->len == v->cap)
    vector_reserve(v, v->cap ? v->cap * 2 : 8);

  memcpy((char *)v->data + v->len * v->elem_size, elem, v->elem_size);

  v->len++;
}

static inline void *vector_at(Vector *v, size_t index) {
  assert(index < v->len);

  return (char *)v->data + index * v->elem_size;
}

#define VECTOR(type) Vector

#define vector_init(type) vector_create(sizeof(type))

#define vector_push_value(v, value)                                            \
  do {                                                                         \
    __typeof__(value) _tmp = (value);                                          \
    vector_push((v), &_tmp);                                                   \
  } while (0)

#define vector_get(type, v, i) (*(type *)vector_at((v), (i)))

#endif