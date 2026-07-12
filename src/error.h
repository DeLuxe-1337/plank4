#pragma once
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#ifndef ERROR_H
#define ERROR_H

typedef enum { ERR_TYPE_RESOLVE = 0, ERR_TYPE_PARSE_ERROR = 1 } ErrorKind;

#define PARSE_ERROR(msg, ...)                                                  \
  {                                                                            \
    fprintf(stderr, msg, __VA_ARGS__);                                         \
    exit(ERR_TYPE_PARSE_ERROR);                                                \
  }

#endif