#pragma once
#include "arena.h"
#include "some.h"
#include "symbol.h"
#include "tokenizer.h"
#include "type.h"
#include "vector.h"
#include <stdalign.h>
#include <stdbool.h>

typedef enum {
  AST_TRANSLATION_UNIT,

  // declarations
  AST_FUNCTION,
  AST_VAR_DECL,
  AST_PARAM,
  AST_TYPEDEF,

  // statements
  AST_BLOCK,
  AST_IF,
  AST_SWITCH,
  AST_CASE,
  AST_WHILE,
  AST_DO_WHILE,
  AST_FOR,
  AST_BREAK,
  AST_CONTINUE,
  AST_RETURN,
  AST_GOTO,
  AST_LABEL,

  // expressions
  AST_BINARY,
  AST_UNARY,
  AST_CALL,
  AST_MEMBER,
  AST_INDEX,
  AST_CAST,
  AST_CONDITIONAL,
  AST_ASSIGN,
  AST_COMPOUND_ASSIGN,
  AST_COMMA,
  AST_SIZEOF_EXPR,
  AST_SIZEOF_TYPE,

  AST_IDENTIFIER,
  AST_INTEGER,
  AST_FLOAT,
  AST_STRING,
  AST_CHAR,

  AST_INIT_LIST,

  AST_EMPTY,
} AstKind;

typedef struct Ast Ast;

struct Ast {
  AstKind kind;

  // SourceLocation loc;

  Type *type; // filled in during semantic analysis

  union {

    struct {
      Vector items;
    } translation_unit;

    struct {
      Ast *decl;
      Ast *body;
    } function;

    struct {
      Vector fields;
    } structure;

    struct {
      Token op;
      Ast *lhs;
      Ast *rhs;
    } binary;

    struct {
      Token op;
      Ast *expr;
    } unary;

    struct {
      Ast *callee;
      Vector args;
    } call;

    struct {
      Ast *cond;
      Ast *then_stmt;
      Ast *else_stmt;
    } if_stmt;

    struct {
      Vector statements;
    } block;

    struct {
      StringView name;
      Symbol *symbol;
    } identifier;

    struct {
      uint64_t value;
    } integer;

    struct {
      Some value;
    } ret;
  };
} __attribute__((aligned(8)));

static void ast_print_indent(int depth) {
  for (int i = 0; i < depth; ++i)
    printf("    ");
}

static const char *ast_kind_name(AstKind kind) {
  switch (kind) {
  case AST_TRANSLATION_UNIT:
    return "TranslationUnit";
  case AST_FUNCTION:
    return "Function";
  case AST_VAR_DECL:
    return "VarDecl";
  case AST_PARAM:
    return "Param";
  case AST_TYPEDEF:
    return "Typedef";

  case AST_BLOCK:
    return "Block";
  case AST_IF:
    return "If";
  case AST_SWITCH:
    return "Switch";
  case AST_CASE:
    return "Case";
  case AST_WHILE:
    return "While";
  case AST_DO_WHILE:
    return "DoWhile";
  case AST_FOR:
    return "For";
  case AST_BREAK:
    return "Break";
  case AST_CONTINUE:
    return "Continue";
  case AST_RETURN:
    return "Return";
  case AST_GOTO:
    return "Goto";
  case AST_LABEL:
    return "Label";

  case AST_BINARY:
    return "Binary";
  case AST_UNARY:
    return "Unary";
  case AST_CALL:
    return "Call";
  case AST_MEMBER:
    return "Member";
  case AST_INDEX:
    return "Index";
  case AST_CAST:
    return "Cast";
  case AST_CONDITIONAL:
    return "Conditional";
  case AST_ASSIGN:
    return "Assign";
  case AST_COMPOUND_ASSIGN:
    return "CompoundAssign";
  case AST_COMMA:
    return "Comma";
  case AST_SIZEOF_EXPR:
    return "SizeofExpr";
  case AST_SIZEOF_TYPE:
    return "SizeofType";

  case AST_IDENTIFIER:
    return "Identifier";
  case AST_INTEGER:
    return "Integer";
  case AST_FLOAT:
    return "Float";
  case AST_STRING:
    return "String";
  case AST_CHAR:
    return "Char";

  case AST_INIT_LIST:
    return "InitList";
  case AST_EMPTY:
    return "Empty";
  }

  return "Unknown";
}

Ast *ast_new(Arena *arena, AstKind kind);

void ast_print(Ast *node, int depth);

Ast *ast_identifier(Arena *arena, StringView name);

Ast *ast_integer(Arena *arena, uint64_t value);

Ast *ast_binary(Arena *arena, Token op, Ast *lhs, Ast *rhs);
Ast *ast_function(Arena *arena, Ast *decl, Ast *body);
Ast *ast_return(Arena *arena, Some value);
Ast *ast_block(Arena *arena);
