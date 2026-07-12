#include "parser.h"
#include "ast.h"
#include "string_view.h"
#include "token.h"
#include "vector.h"

Ast *parse_return_stmt(Parser *p) {
  expect(p, "ret semicolon", TOK_SEMICOLON);
  return ast_return(p->arena, some(NULL));
}
Ast *parse_function_stmt(Parser *p) {
  Token identifier = expect(p, "function stmt", TOK_IDENTIFIER);
  Ast *decl = ast_identifier(
      p->arena, sv_from_parts(identifier.begin, identifier.length));

  Ast *body = parse_block_stmt(p);

  return ast_function(p->arena, decl, body);
}
Ast *parse_block_stmt(Parser *p) {

  expect(p, "block stmt L brace", TOK_LBRACE);

  Ast *stmt = ast_block(p->arena);

  printf("Block Start\n");

  while (!match(p, TOK_RBRACE)) {
    Ast *body_stmt = parse_stmt(p);
    ast_print(body_stmt, 0);
    vector_push(&stmt->block.statements, &body_stmt);
  }

  printf("Block End\n");

  return stmt;
}
Ast *parse_stmt(Parser *p) {
  if (p->eof)
    return NULL;
  printf("Current Statement Kind : %s\n", token_kind_name(p->current.kind));
  if (match(p, TOK_FUNCTION)) {
    return parse_function_stmt(p);
  }
  if (match(p, TOK_RETURN)) {
    return parse_return_stmt(p);
  }

  advance(p);

  return parse_stmt(p);
}
