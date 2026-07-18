#pragma once

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
  X(STRUCT)                                                                    \
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

static const char *token_kind_name(TokenKind kind) {
  if ((unsigned)kind >= TOK_COUNT)
    return "INVALID_TOKEN";

  return token_kind_names[kind];
}
