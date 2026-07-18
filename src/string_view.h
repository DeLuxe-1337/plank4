#pragma once
#include "arena.h"

#include <stdbool.h>
#include <stddef.h>
#include <string.h>

typedef struct {
  const char *data;
  size_t len;
} StringView;

#define SV(str) ((StringView){(str), sizeof(str) - 1})

static inline StringView sv_from_parts(const char *data, size_t len) {
  return (StringView){data, len};
}

static inline StringView sv_from_cstr(const char *str) {
  return (StringView){str, strlen(str)};
}

static inline bool sv_equal(StringView a, StringView b) {
  return a.len == b.len && memcmp(a.data, b.data, a.len) == 0;
}

static inline bool sv_equal_cstr(StringView a, const char *b) {
  return sv_equal(a, sv_from_cstr(b));
}

static char *arena_sv_to_cstr(Arena *arena, StringView sv) {
  char *s = (char *)arena_alloc(arena, sv.len + 1);

  memcpy(s, sv.data, sv.len);
  s[sv.len] = '\0';

  return s;
}
