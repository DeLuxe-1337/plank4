#pragma once

#include "arena.h"
#include "ast.h"
#include "error.h"
#include "some.h"
#include "token.h"

#ifndef PARSER_H
#define PARSER_H

typedef struct {
  Lexer *lexer;
  Token current;
  Arena *arena;
  bool eof;
} Parser;

static Parser *parser_create(Arena *arena, Lexer *lexer) {
  Parser *p = arena_new_zero(arena, Parser);
  p->lexer = lexer;
  p->current = lexer_next(lexer);
  p->arena = arena;
  p->eof = false;

  return p;
}

static Token *peek(Parser *p) { return &p->current; }

static Token advance(Parser *p) {
  if (p->current.kind == TOK_EOF || p->eof == true) {
    p->eof = true;
    return p->current;
  }
  Token old = p->current;
  p->current = lexer_next(p->lexer);
  printf(" <- Advanced('%.*s')->Kind(%s)\n", (int)old.length, old.begin,
         token_kind_name(old.kind));
  if (p->current.kind == TOK_EOF)
    return old;

  return old;
}

static bool check(Parser *p, TokenKind kind) { return peek(p)->kind == kind; }

static bool match(Parser *p, TokenKind kind) {
  if (check(p, kind)) {
    printf("-> Matched('%.*s') Expected Token Kind(%s)\n", p->current.length,
           p->current.begin, token_kind_name(kind));
    advance(p);
    return true;
  }
  return false;
}

static Token expect(Parser *p, const char *expected, TokenKind kind) {
  printf("Consumed('%.*s') Expected Token Kind(%s) Expected(%s)\n",
         p->current.length, p->current.begin, token_kind_name(kind), expected);
  Token old = p->current;
  if (!match(p, kind))
    PARSE_ERROR("Expected token of kind %s", token_kind_name(kind));
  return old;
}

Some parse_expr(Parser *p);
Ast *parse_stmt(Parser *p);
Ast *parse_return_stmt(Parser *p);
Ast *parse_function_stmt(Parser *p);
Ast *parse_block_stmt(Parser *p);

#endif