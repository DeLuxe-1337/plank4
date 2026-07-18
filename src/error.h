#pragma once
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

typedef enum { ERR_TYPE_RESOLVE = 0, ERR_TYPE_PARSE_ERROR = 1 } ErrorKind;

#define PARSE_ERROR(msg, ...)                                                  \
  {                                                                            \
    fprintf(stderr, msg, __VA_ARGS__);                                         \
    exit(ERR_TYPE_PARSE_ERROR);                                                \
  }
