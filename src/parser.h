#pragma once

#include "arena.h"
#include "ast.h"
#include "error.h"
#include "token.h"

#ifndef PARSER_H
#define PARSER_H

typedef struct {
  Lexer *lexer;
  Token current;
  Arena *arena;
} Parser;

static Parser *parser_create(Arena *arena, Lexer *lexer) {
  Parser *p = arena_new_zero(arena, Parser);
  p->lexer = lexer;
  p->current = lexer_next(lexer);
  p->arena = arena;

  return p;
}

static Token *peek(Parser *p) { return &p->current; }

static Token advance(Parser *p) {
  Token old = p->current;
  p->current = lexer_next(p->lexer);
  return old;
}

static bool check(Parser *p, TokenKind kind) { return peek(p)->kind == kind; }

static bool match(Parser *p, TokenKind kind) {
  if (check(p, kind)) {
    advance(p);
    return true;
  }
  return false;
}

static Token expect(Parser *p, TokenKind kind) {
  if (!match(p, kind))
    PARSE_ERROR("Expected token of kind %d", kind);
  return p->current;
}

Ast *parse_stmt(Parser *p);
Ast *parse_return_stmt(Parser *p);
Ast *parse_function_stmt(Parser *p);

#endif