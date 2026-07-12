#pragma once

#include "string_view.h"
#include "type.h"

typedef struct Ast Ast;
typedef struct Scope Scope;

typedef enum {
  SYMBOL_VARIABLE,
  SYMBOL_FUNCTION,
  SYMBOL_TYPEDEF,
  SYMBOL_ENUM_CONSTANT,
  SYMBOL_STRUCT,
  SYMBOL_UNION,
  SYMBOL_ENUM,
  SYMBOL_LABEL,
} SymbolKind;

typedef struct Symbol {
  SymbolKind kind;
  StringView name;

  Type *type;

  Ast *declaration;

  Scope *scope;

} Symbol;
