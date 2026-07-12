#pragma once
#include "string_view.h"
#include "vector.h"
#ifndef TYPE_H
#define TYPE_H

typedef enum {
  TYPE_VOID,
  TYPE_BOOL,

  TYPE_CHAR,
  TYPE_SHORT,
  TYPE_INT,
  TYPE_LONG,

  TYPE_FLOAT,
  TYPE_DOUBLE,

  TYPE_POINTER,
  TYPE_ARRAY,
  TYPE_FUNCTION,

  TYPE_STRUCT,
  TYPE_UNION,
  TYPE_ENUM,
} TypeKind;

typedef struct Type Type;

struct Type {
  TypeKind kind;

  union {
    struct {
      Type *base;
    } pointer;

    struct {
      Type *base;
      size_t length;
    } array;

    struct {
      Type *return_type;
      Vector params;
      bool variadic;
    } function;

    struct {
      StringView name;
      Vector fields;
    } record;
  };
};

#endif