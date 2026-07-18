#pragma once

#include <stddef.h>

#include "string_view.h"
#include <stdbool.h>

typedef struct {
  StringView key;
  void *value;

  bool occupied;
} HashEntry;

typedef struct {
  HashEntry *entries;

  size_t len;
  size_t cap;
} HashMap;

void hashmap_init(HashMap *map);

void hashmap_destroy(HashMap *map);

HashEntry *hashmap_find(HashMap *map, StringView key);

void *hashmap_get(HashMap *map, StringView key);

void hashmap_put(HashMap *map, StringView key, void *value);

bool hashmap_contains(HashMap *map, StringView key);
