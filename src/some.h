#pragma once
#include <stdbool.h>

typedef struct {
  void *ptr;
  bool nil;
} Some;

Some some(void *value);

typedef Some SomeValueRef;
