#include "type.h"

Type *type_new(Arena *arena, TypeKind kind) {
  Type *t =
      (Type *)arena_alloc(arena, sizeof(Type)); // Allocate memory from the are
  t->kind = kind;
  return t;
}
