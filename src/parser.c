#include "parser.h"
#include "ast.h"
#include "string_view.h"

Ast *parse_return_stmt(Parser *p) { return ast_return(p->arena, some(NULL)); }
Ast *parse_function_stmt(Parser *p) {
  Token identifier = advance(p);
  return ast_function(
      p->arena,
      ast_identifier(p->arena,
                     sv_from_parts(identifier.begin, identifier.length)),
      NULL);
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
