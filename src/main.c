#include "arena.h"
#include "ast.h"
#include "tokenizer.h"
#include <stdio.h>

int main(void) {
  printf("Hello, world\n");
  const char *source = "function main {}";

  Lexer lex = {
      .start = source,
      .current = source,
      .line = 1,
      .column = 1,
  };

  Arena arena;
  arena_init(&arena, sizeof(Ast));

  Token plus = make_token(&lex, TOK_PLUS, NULL, 0, 0);

  Ast *expr = ast_binary(&arena, plus, ast_identifier(&arena, SV("x")),
                         ast_integer(&arena, 5));
  printf("Hello, world\n");

  ast_print(expr, 0);

  Token tok;

  do {
    tok = lexer_next(&lex);

    printf("%-20s '%.*s' (%u:%u)\n", token_kind_name(tok.kind), (int)tok.length,
           tok.begin, tok.line, tok.column);

  } while (tok.kind != TOK_EOF);

  arena_destroy(&arena);

  return 0;
}