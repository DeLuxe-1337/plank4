#include "tokenizer.h"
#include <stdio.h>

int main(void) { 
  const char *source = "function main {}";

  Lexer lex = {
      .start = source,
      .current = source,
      .line = 1,
      .column = 1,
  };

  Token tok;

  do {
    tok = lexer_next(&lex);

    printf("%-20s '%.*s' (%u:%u)\n", token_kind_name(tok.kind), (int)tok.length,
           tok.begin, tok.line, tok.column);

  } while (tok.kind != TOK_EOF);

  return 0;
}