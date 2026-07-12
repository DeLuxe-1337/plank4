// #pragma once
#include "tokenizer.h"
#include <string.h>

int is_alpha(char c) {
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

int is_digit(char c) { return c >= '0' && c <= '9'; }
int is_alnum(char c) { return is_alpha(c) || is_digit(c); }

char peek(Lexer *lex) { return *lex->current; }

char peek_next(Lexer *lex) {
  if (lex->current[0] == '\0')
    return '\0';

  return lex->current[1];
}

char advance(Lexer *lex) {
  char c = *lex->current++;

  if (c == '\n') {
    lex->line++;
    lex->column = 1;
  } else {
    lex->column++;
  }

  return c;
}

int match(Lexer *lex, char expected) {
  if (*lex->current != expected)
    return 0;

  advance(lex);
  return 1;
}

Token make_token(Lexer *lex, TokenKind kind, const char *begin, unsigned line,
                 unsigned column) {
  Token tok;

  tok.kind = kind;
  tok.begin = begin;
  tok.length = (size_t)(lex->current - begin);
  tok.line = line;
  tok.column = column;

  return tok;
}

void skip_space(Lexer *lex) {
  for (;;) {

    switch (peek(lex)) {

    case ' ':
    case '\t':
    case '\r':
      advance(lex);
      break;

    case '\n':
      advance(lex);
      break;

    case '/':
      if (peek_next(lex) == '/') {

        while (peek(lex) != '\n' && peek(lex) != '\0')
          advance(lex);
      } else if (peek_next(lex) == '*') {

        advance(lex);
        advance(lex);

        while (peek(lex) != '\0') {

          if (peek(lex) == '*' && peek_next(lex) == '/') {

            advance(lex);
            advance(lex);
            break;
          }

          advance(lex);
        }
      } else {
        return;
      }

      break;

    default:
      return;
    }
  }
}

Token identifier(Lexer *lex) {
  const char *begin = lex->current;
  unsigned line = lex->line;
  unsigned column = lex->column;

  while (is_alnum(peek(lex)))
    advance(lex);

  if (memcmp(begin, "function", 8) == 0)
    return make_token(lex, TOK_FUNCTION, begin, line, column);

  return make_token(lex, TOK_IDENTIFIER, begin, line, column);
}

Token number(Lexer *lex) {
  const char *begin = lex->current;
  unsigned line = lex->line;
  unsigned column = lex->column;

  while (is_digit(peek(lex)))
    advance(lex);

  if (peek(lex) == '.' && is_digit(peek_next(lex))) {

    advance(lex);

    while (is_digit(peek(lex)))
      advance(lex);

    return make_token(lex, TOK_FLOAT, begin, line, column);
  }

  return make_token(lex, TOK_INTEGER, begin, line, column);
}

Token lexer_next(Lexer *lex) {
  skip_space(lex);

  const char *begin = lex->current;
  unsigned line = lex->line;
  unsigned column = lex->column;

  char c = advance(lex);

  switch (c) {

  case '\0':
    return make_token(lex, TOK_EOF, begin, line, column);
  case '{':
    return make_token(lex, TOK_LBRACE, begin, line, column);

  case '}':
    return make_token(lex, TOK_RBRACE, begin, line, column);
  case '(':
    return make_token(lex, TOK_LPAREN, begin, line, column);

  case ')':
    return make_token(lex, TOK_RPAREN, begin, line, column);

  case '+':
    if (match(lex, '+'))
      return make_token(lex, TOK_PLUS_PLUS, begin, line, column);

    if (match(lex, '='))
      return make_token(lex, TOK_PLUS_EQUAL, begin, line, column);

    return make_token(lex, TOK_PLUS, begin, line, column);

  case '-':
    if (match(lex, '>'))
      return make_token(lex, TOK_ARROW, begin, line, column);

    if (match(lex, '-'))
      return make_token(lex, TOK_MINUS_MINUS, begin, line, column);

    if (match(lex, '='))
      return make_token(lex, TOK_MINUS_EQUAL, begin, line, column);

    return make_token(lex, TOK_MINUS, begin, line, column);
  }

  if (is_alpha(c)) {
    lex->current--;
    lex->column--;
    return identifier(lex);
  }

  if (is_digit(c)) {
    lex->current--;
    lex->column--;
    return number(lex);
  }

  return make_token(lex, TOK_UNKNOWN, begin, line, column);
}
const char *token_kind_name(TokenKind kind) {
  if ((unsigned)kind >= TOK_COUNT)
    return "INVALID_TOKEN";

  return token_kind_names[kind];
}
