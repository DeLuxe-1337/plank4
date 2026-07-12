#pragma once
#ifndef HASH_H
#define HASH_H

#include "string_view.h"
#include <stddef.h>
#include <stdint.h>

static uint32_t hash(StringView view) {
  uint32_t h = 2166136261u;

  for (size_t i = 0; i < view.len; i++) {
    h ^= (unsigned char)view.data[i];
    h *= 16777619u;
  }

  return h;
}

#endif