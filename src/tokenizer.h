// #pragma once
#ifndef TOKENIZER_H
#define TOKENIZER_H
#include "hash.h"
#include "token.h"
#include <stddef.h>

typedef struct {
  TokenKind kind;

  const char *begin;
  size_t length;

  unsigned line;
  unsigned column;
} Token;

typedef struct {
  const char *start;
  const char *current;

  unsigned line;
  unsigned column;
} Lexer;

static int is_alpha(char c);

static int is_digit(char c);

static int is_alnum(char c);

static char l_peek(Lexer *lex);

static char l_peek_next(Lexer *lex);

static char l_advance(Lexer *lex);

static int l_match(Lexer *lex, char expected);

Token make_token(Lexer *lex, TokenKind kind, const char *begin, unsigned line,
                 unsigned column);

static void skip_space(Lexer *lex);

static Token identifier(Lexer *lex);

static Token number(Lexer *lex);

Token lexer_next(Lexer *lex);

#endif