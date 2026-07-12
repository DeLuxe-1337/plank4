// #pragma once
#include "tokenizer.h"
#include <string.h>

int is_alpha(char c) {
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

int is_digit(char c) { return c >= '0' && c <= '9'; }
int is_alnum(char c) { return is_alpha(c) || is_digit(c); }

char l_peek(Lexer *lex) { return *lex->current; }

char l_peek_next(Lexer *lex) {
  if (lex->current[0] == '\0')
    return '\0';

  return lex->current[1];
}

char l_advance(Lexer *lex) {
  char c = *lex->current++;

  if (c == '\n') {
    lex->line++;
    lex->column = 1;
  } else {
    lex->column++;
  }

  return c;
}

int l_match(Lexer *lex, char expected) {
  if (*lex->current != expected)
    return 0;

  l_advance(lex);
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

    switch (l_peek(lex)) {

    case ' ':
    case '\t':
    case '\r':
      l_advance(lex);
      break;

    case '\n':
      l_advance(lex);
      break;

    case '/':
      if (l_peek_next(lex) == '/') {

        while (l_peek(lex) != '\n' && l_peek(lex) != '\0')
          l_advance(lex);
      } else if (l_peek_next(lex) == '*') {

        l_advance(lex);
        l_advance(lex);

        while (l_peek(lex) != '\0') {

          if (l_peek(lex) == '*' && l_peek_next(lex) == '/') {

            l_advance(lex);
            l_advance(lex);
            break;
          }

          l_advance(lex);
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

  while (is_alnum(l_peek(lex)))
    l_advance(lex);

  if (memcmp(begin, "function", 8) == 0)
    return make_token(lex, TOK_FUNCTION, begin, line, column);

  if (memcmp(begin, "return", 6) == 0)
    return make_token(lex, TOK_RETURN, begin, line, column);

  return make_token(lex, TOK_IDENTIFIER, begin, line, column);
}

Token number(Lexer *lex) {
  const char *begin = lex->current;
  unsigned line = lex->line;
  unsigned column = lex->column;

  while (is_digit(l_peek(lex)))
    l_advance(lex);

  if (l_peek(lex) == '.' && is_digit(l_peek_next(lex))) {

    l_advance(lex);

    while (is_digit(l_peek(lex)))
      l_advance(lex);

    return make_token(lex, TOK_FLOAT, begin, line, column);
  }

  return make_token(lex, TOK_INTEGER, begin, line, column);
}

Token lexer_next(Lexer *lex) {
  skip_space(lex);

  const char *begin = lex->current;
  unsigned line = lex->line;
  unsigned column = lex->column;

  char c = l_advance(lex);

  switch (c) {
  case ';':
    return make_token(lex, TOK_SEMICOLON, begin, line, column);
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
    if (l_match(lex, '+'))
      return make_token(lex, TOK_PLUS_PLUS, begin, line, column);

    if (l_match(lex, '='))
      return make_token(lex, TOK_PLUS_EQUAL, begin, line, column);

    return make_token(lex, TOK_PLUS, begin, line, column);

  case '-':
    if (l_match(lex, '>'))
      return make_token(lex, TOK_ARROW, begin, line, column);

    if (l_match(lex, '-'))
      return make_token(lex, TOK_MINUS_MINUS, begin, line, column);

    if (l_match(lex, '='))
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