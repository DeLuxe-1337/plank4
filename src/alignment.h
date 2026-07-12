#pragma once

typedef union {
  void *p;
  long l;
  long long ll;
  double d;
  long double ld;
} ArenaAlign;

#define ARENA_ALIGNMENT sizeof(ArenaAlign)