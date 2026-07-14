#include "visitor.h"
#include "ast.h"
#include "error.h"
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

LLVMValueRef resolve_default_void() { return LLVMConstPointerNull(LLVMVoidType()); }

LLVMTypeRef resolve_typekind(TypeKind kind) {
  switch (kind) {
  case TYPE_VOID:
    return LLVMTypeOf(resolve_default_void());
  case TYPE_INT:
    return LLVMIntType(8);
  case TYPE_FLOAT:
    return LLVMFloatType();
  default:
    fprintf(stderr, "Unable to find type in resolve_type\n");
    exit(ERR_TYPE_RESOLVE);
  }
}

LLVMValueRef visit_expr(Visitor *v, Ast *n) {
  LLVMValueRef retv = resolve_default_void();
  printf("-> visit expr(%s)\n", ast_kind_name(n->kind));
  ast_print(n, 0);
  switch (n->kind) {
  case AST_INTEGER:
    retv = LLVMConstInt(resolve_typekind(TYPE_INT), n->integer.value, false);
    break;
  }
  char *retv_llvm_message = LLVMPrintValueToString(retv);
  printf("<- expr retv(%s)\n", retv_llvm_message);
  LLVMDisposeMessage(retv_llvm_message);
  return retv;
}

void visit_stmt(Visitor *v, Ast *n) {
  printf("-> visit stmt(%s)\n", ast_kind_name(n->kind));
  ast_print(n, 0);

  switch (n->kind) {

  case AST_FUNCTION:; // solve c23 ext warning
    char *name = arena_sv_to_cstr(v->arena, n->function.decl->identifier.name);

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

    // if (returnType == LLVMVoidType()) {
    //   LLVMBuildRetVoid(v->builder);
    // }

    break;

  case AST_BLOCK:
    for (size_t i = 0; i < n->block.statements.len; i++) {
      Ast *x = vector_get(Ast *, &n->block.statements, i);

      visit_stmt(v, x);
    }
    vector_free(&n->block.statements);
    break;
  case AST_RETURN:
    Some expr = n->ret.value;
    LLVMValueRef value = resolve_default_void();

    if (!expr.nil) {
      value = visit_expr(v, expr.ptr);
    }

    LLVMBuildRet(v->builder, value);

    break;

  default:
    break;
  }
}
