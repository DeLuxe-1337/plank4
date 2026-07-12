#include "hash_map.h"

#include "hash.h"

#include <assert.h>
#include <stdlib.h>

static void hashmap_grow(HashMap *map);

void hashmap_init(HashMap *map) {
  map->cap = 64;
  map->len = 0;

  map->entries = (HashEntry *)calloc(map->cap, sizeof(HashEntry));

  assert(map->entries);
}

void hashmap_destroy(HashMap *map) {
  free(map->entries);

  map->entries = NULL;
  map->len = 0;
  map->cap = 0;
}

HashEntry *hashmap_find(HashMap *map, StringView key) {
  uint64_t h = hash(key);

  size_t i = h & (map->cap - 1);

  for (;;) {

    HashEntry *e = &map->entries[i];

    if (!e->occupied)
      return e;

    if (sv_equal(e->key, key))
      return e;

    i = (i + 1) & (map->cap - 1);
  }
}

void *hashmap_get(HashMap *map, StringView key) {
  HashEntry *e = hashmap_find(map, key);

  if (!e->occupied)
    return NULL;

  return e->value;
}

static void hashmap_grow(HashMap *map) {
  HashEntry *old = map->entries;
  size_t old_cap = map->cap;

  map->cap *= 2;
  map->len = 0;

  map->entries = (HashEntry *)calloc(map->cap, sizeof(HashEntry));

  assert(map->entries);

  for (size_t i = 0; i < old_cap; ++i) {

    if (!old[i].occupied)
      continue;

    hashmap_put(map, old[i].key, old[i].value);
  }

  free(old);
}

void hashmap_put(HashMap *map, StringView key, void *value) {
  if (map->len * 100 / map->cap >= 70)
    hashmap_grow(map);

  HashEntry *e = hashmap_find(map, key);

  if (!e->occupied) {

    e->occupied = true;
    e->key = key;

    map->len++;
  }

  e->value = value;
}

bool hashmap_contains(HashMap *map, StringView key) {
  HashEntry *e = hashmap_find(map, key);

  return e->occupied;
}