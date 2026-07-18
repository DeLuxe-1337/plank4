#ifndef SOME_H
#define SOME_H

#include <stdbool.h>

typedef struct {
  void *ptr;
  bool nil;
} Some;

Some some(void *value);

typedef Some SomeValueRef;

#endif