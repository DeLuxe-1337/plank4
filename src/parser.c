#include "parser.h"
#include "ast.h"
#include "string_view.h"
#include "token.h"

Ast *parse_return_stmt(Parser *p) { return ast_return(p->arena, some(NULL)); }
Ast *parse_function_stmt(Parser *p) {
  Token identifier = expect(p, TOK_IDENTIFIER);
  Ast *decl = ast_identifier(
      p->arena, sv_from_parts(identifier.begin, identifier.length));

  expect(p, TOK_LBRACE);
  expect(p, TOK_RBRACE);

  return ast_function(p->arena, decl, NULL);
}
Ast *parse_stmt(Parser *p) {
  if (match(p, TOK_FUNCTION)) {
    return parse_function_stmt(p);
  }
  if (match(p, TOK_RETURN)) {
    return parse_return_stmt(p);
  }

  advance(p);

  return parse_stmt(p);
}
