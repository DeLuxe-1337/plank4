#pragma once
#ifndef HASH_H
#define HASH_H

#include <stddef.h>
#include <stdint.h>

static uint32_t hash(const char *str, size_t len) {
  uint32_t h = 2166136261u;

  for (size_t i = 0; i < len; i++) {
    h ^= (unsigned char)str[i];
    h *= 16777619u;
  }

  return h;
}

#endif