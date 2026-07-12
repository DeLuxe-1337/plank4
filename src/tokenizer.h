// #pragma once
#ifndef TOKEN_H
#define TOKEN_H
#include "hash.h"
#include <stddef.h>

#define TOKEN_LIST(X)                                                          \
  X(EOF)                                                                       \
                                                                               \
  /* identifiers */                                                            \
  X(IDENTIFIER)                                                                \
                                                                               \
  /* literals */                                                               \
  X(INTEGER)                                                                   \
  X(FLOAT)                                                                     \
  X(STRING)                                                                    \
  X(CHAR)                                                                      \
                                                                               \
  /* keywords */                                                               \
  X(IF)                                                                        \
  X(ELSE)                                                                      \
  X(FOR)                                                                       \
  X(WHILE)                                                                     \
  X(RETURN)                                                                    \
  X(FUNCTION)                                                                  \
                                                                               \
  /* punctuation */                                                            \
  X(LPAREN)                                                                    \
  X(RPAREN)                                                                    \
  X(LBRACE)                                                                    \
  X(RBRACE)                                                                    \
  X(LBRACKET)                                                                  \
  X(RBRACKET)                                                                  \
  X(COMMA)                                                                     \
  X(SEMICOLON)                                                                 \
  X(COLON)                                                                     \
  X(DOT)                                                                       \
                                                                               \
  /* operators */                                                              \
  X(PLUS)                                                                      \
  X(MINUS)                                                                     \
  X(STAR)                                                                      \
  X(SLASH)                                                                     \
  X(PERCENT)                                                                   \
                                                                               \
  X(EQUAL)                                                                     \
  X(EQUAL_EQUAL)                                                               \
                                                                               \
  X(BANG)                                                                      \
  X(BANG_EQUAL)                                                                \
                                                                               \
  X(LESS)                                                                      \
  X(LESS_EQUAL)                                                                \
                                                                               \
  X(GREATER)                                                                   \
  X(GREATER_EQUAL)                                                             \
                                                                               \
  X(AND)                                                                       \
  X(AND_AND)                                                                   \
                                                                               \
  X(OR)                                                                        \
  X(OR_OR)                                                                     \
                                                                               \
  X(XOR)                                                                       \
  X(TILDE)                                                                     \
                                                                               \
  X(PLUS_PLUS)                                                                 \
  X(MINUS_MINUS)                                                               \
                                                                               \
  X(PLUS_EQUAL)                                                                \
  X(MINUS_EQUAL)                                                               \
  X(STAR_EQUAL)                                                                \
  X(SLASH_EQUAL)                                                               \
                                                                               \
  X(ARROW)                                                                     \
                                                                               \
  X(UNKNOWN)

typedef enum TokenKind {
#define X(name) TOK_##name,
  TOKEN_LIST(X)
#undef X

      TOK_COUNT
} TokenKind;

static const char *const token_kind_names[TOK_COUNT] = {
#define X(name) [TOK_##name] = #name,
    TOKEN_LIST(X)
#undef X
};

const char *token_kind_name(TokenKind kind);

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

static char peek(Lexer *lex);

static char peek_next(Lexer *lex);

static char advance(Lexer *lex);

static int match(Lexer *lex, char expected);

Token make_token(Lexer *lex, TokenKind kind, const char *begin,
                        unsigned line, unsigned column);

static void skip_space(Lexer *lex);

static Token identifier(Lexer *lex);

static Token number(Lexer *lex);

Token lexer_next(Lexer *lex);

#endif