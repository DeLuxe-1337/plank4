#include "visitor.h"
#include "ast.h"
#include "string_view.h"
#include "type.h"
#include <llvm-c-21/llvm-c/Core.h>
#include <stdio.h>

Visitor *visitor_create(Arena *arena, const char *name) {
  Visitor *v = arena_alloc(arena, sizeof(Visitor));
  v->module = LLVMModuleCreateWithName(name);
  v->builder = LLVMCreateBuilder();
  v->arena = arena;
  return v;
}

void visitor_cleanup(Visitor *v) {
  LLVMDisposeBuilder(v->builder);
  LLVMDisposeModule(v->module);
}

Some visit_expr(Visitor *v, Ast *n) { return some(NULL, true); }

LLVMTypeRef resolve_typekind(TypeKind kind) {
  switch (kind) {
  case TYPE_VOID:
    return LLVMVoidType();
  case TYPE_INT:
    return LLVMInt32Type();
  case TYPE_FLOAT:
    return LLVMFloatType();
  default:
    fprintf(stderr, "Unable to find type in resolve_type\n");
    abort();
  }
}

void visit_stmt(Visitor *v, Ast *n) {
  ast_print(n, 0);

  switch (n->kind) {

  case AST_FUNCTION:; // solve c23 ext warning
    char *name = arena_sv_to_cstr(v->arena, n->function.decl->identifier.name);
    printf("function %s\n", name);

    bool isVararg = n->type->function.variadic;
    LLVMTypeRef returnType =
        resolve_typekind(n->type->function.return_type->kind);
    LLVMTypeRef functionType =
        LLVMFunctionType(returnType, NULL, 0, (int)isVararg);

    LLVMValueRef function = LLVMAddFunction(v->module, name, functionType);

    LLVMBasicBlockRef entry = LLVMAppendBasicBlock(function, "entry");

    LLVMPositionBuilderAtEnd(v->builder, entry);

    if (n->function.body != NULL)
      visit_stmt(v, n->function.body);

    if (returnType == LLVMVoidType()) {
      LLVMBuildRetVoid(v->builder);
    }

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
