#pragma once
#include "arena.h"
#include "bool.h"
#include "some.h"
#include "symbol.h"
#include "tokenizer.h"
#include "type.h"
#include "vector.h"
#ifndef AST_H
#define AST_H

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
  };
};

Ast *ast_new(Arena *arena, AstKind kind);

void ast_print(Ast *node, int depth);

Ast *ast_identifier(Arena *arena, StringView name);

Ast *ast_integer(Arena *arena, uint64_t value);

Ast *ast_binary(Arena *arena, Token op, Ast *lhs, Ast *rhs);
Ast *ast_function(Arena *arena, Ast *decl, Ast *body);

#endif