#pragma once

#include <assert.h>

#include "arena.h"
#include "hash_map.h"
#include "string_view.h"
#include "symbol.h"

typedef struct Scope Scope;

typedef enum {
  SCOPE_ORDINARY,
  SCOPE_TAG,
  SCOPE_LABEL,
} ScopeNamespace;

struct Scope {
  Scope *parent;

  HashMap ordinary;
  HashMap tags;
  HashMap labels;
};

typedef struct {
  Arena *arena;

  Scope *global;
  Scope *current;
} SemanticContext;

static inline Scope *scope_create(Arena *arena, Scope *parent) {
  Scope *scope = arena_new(arena, Scope);

  scope->parent = parent;

  hashmap_init(&scope->ordinary);
  hashmap_init(&scope->tags);
  hashmap_init(&scope->labels);

  return scope;
}

static inline void sema_init(SemanticContext *ctx, Arena *arena) {
  ctx->arena = arena;

  ctx->global = scope_create(arena, NULL);
  ctx->current = ctx->global;
}

static inline void scope_push(SemanticContext *ctx) {
  ctx->current = scope_create(ctx->arena, ctx->current);
}

static inline void scope_pop(SemanticContext *ctx) {
  assert(ctx->current->parent);
  ctx->current = ctx->current->parent;
}

static inline bool scope_insert(Scope *scope, Symbol *sym) {
  HashMap *table;

  switch (sym->kind) {

  case SYMBOL_STRUCT:
  case SYMBOL_UNION:
  case SYMBOL_ENUM:
    table = &scope->tags;
    break;

  case SYMBOL_LABEL:
    table = &scope->labels;
    break;

  default:
    table = &scope->ordinary;
    break;
  }

  if (hashmap_contains(table, sym->name))
    return false;

  hashmap_put(table, sym->name, sym);

  sym->scope = scope;

  return true;
}

static inline Symbol *scope_lookup(Scope *scope, ScopeNamespace ns,
                                   StringView name) {
  while (scope) {

    HashMap *table;

    switch (ns) {

    case SCOPE_ORDINARY:
      table = &scope->ordinary;
      break;

    case SCOPE_TAG:
      table = &scope->tags;
      break;

    case SCOPE_LABEL:
      table = &scope->labels;
      break;

    default:
      return NULL;
    }

    Symbol *sym = (Symbol *)hashmap_get(table, name);

    if (sym)
      return sym;

    scope = scope->parent;
  }

  return NULL;
}

static inline bool declare_symbol(SemanticContext *ctx, Symbol *sym) {
  return scope_insert(ctx->current, sym);
}

static inline Symbol *lookup_symbol(SemanticContext *ctx, StringView name) {
  return scope_lookup(ctx->current, SCOPE_ORDINARY, name);
}

static inline Symbol *lookup_tag(SemanticContext *ctx, StringView name) {
  return scope_lookup(ctx->current, SCOPE_TAG, name);
}

static inline Symbol *lookup_label(SemanticContext *ctx, StringView name) {
  return scope_lookup(ctx->current, SCOPE_LABEL, name);
}
