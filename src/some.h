#ifndef SOME_H
#define SOME_H

#include "bool.h"

typedef struct {
  void *ptr;
  bool nil;
} Some;

Some some(void *value);

#endif