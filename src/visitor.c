#include "visitor.h"

Visitor *visitor_create(const char *name) {
  Visitor *v = malloc(sizeof(Visitor));
  v->module = LLVMModuleCreateWithName(name);
  v->builder = LLVMCreateBuilder();
  return v;
}

Some visit_expr(Visitor *v, Ast *n) { return some(NULL); }

void visit_stmt(Visitor *v, Ast *n) {
  switch (n->kind) {

  case AST_FUNCTION:
    const char *name = n->function.decl->identifier.name.data +
                       n->function.decl->identifier.name.len;

    
    break;

  case AST_BLOCK:
    for (size_t i = 0; i < n->block.statements.len; i++) {
      Ast *x = (Ast *)vector_at(&n->block.statements, i);
      visit_stmt(v, x);
    }
    break;

  default:
    break;
  }
}
