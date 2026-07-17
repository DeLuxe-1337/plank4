#include "visitor.h"
#include "ast.h"
#include "error.h"
#include "string_view.h"
#include "symbol.h"
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

LLVMTypeRef resolve_typekind(TypeKind kind) {
  switch (kind) {
  case TYPE_VOID:
    return LLVMVoidType();
  case TYPE_INT:
    return LLVMIntType(32);
  case TYPE_FLOAT:
    return LLVMFloatType();
  default:
    fprintf(stderr, "Unable to find type in resolve_type\n");
    exit(ERR_TYPE_RESOLVE);
  }
}

SomeValueRef visit_expr(Visitor *v, Ast *n) {
  SomeValueRef retv = some(NULL);
  printf("-> visit expr(%s)\n", ast_kind_name(n->kind));
  ast_print(n, 0);
  switch (n->kind) {
  case AST_INTEGER:
    retv.ptr =
        LLVMConstInt(resolve_typekind(TYPE_INT), n->integer.value, false);
    retv.nil = false;
    break;
  }
  char *retv_llvm_message = LLVMPrintValueToString(retv.ptr);
  printf("<- expr retv(%s)\n", retv_llvm_message);
  LLVMDisposeMessage(retv_llvm_message);
  return retv;
}

LLVMTypeRef get_return_type(Ast *n) {
  if (n->block.statements.len == 0)
    goto RET_VOID;

  for (size_t i = 0; i < n->block.statements.len; i++) {
    Ast *x = vector_get(Ast *, &n->block.statements, i);
    if (x != NULL && x->kind == AST_RETURN && x->ret.value.nil == false) {
      Ast *ret = ((Ast *)x->ret.value.ptr);
      switch (ret->kind) {
      case AST_INTEGER:
        return resolve_typekind(TYPE_INT);
      default:
        return resolve_typekind(TYPE_VOID);
      }
    }
  }

RET_VOID:

  return resolve_typekind(TYPE_VOID);
}

bool validate_return_state(Visitor *v, Ast *n) {
  if (n->block.statements.len == 0)
    goto PUSH_RET;

  for (size_t i = 0; i < n->block.statements.len; i++) {
    Ast *x = vector_get(Ast *, &n->block.statements, i);
    if (x != NULL && x->kind == AST_RETURN && x->ret.value.nil == false) {
      Ast *ret = ((Ast *)x->ret.value.ptr);
      switch (ret->kind) {
      case AST_RETURN:
        return true;
      }
    } else if (i == n->block.statements.len - 1) {
    PUSH_RET:
      Ast *default_ret = ast_return(v->arena, some(NULL));
      vector_push(&n->block.statements, &default_ret);
      return false;
    }
  }

  return false;
}

void visit_stmt(Visitor *v, Ast *n) {
  printf("-> visit stmt(%s)\n", ast_kind_name(n->kind));
  ast_print(n, 0);

  switch (n->kind) {

  case AST_FUNCTION:; // solve c23 ext warning
    char *name = arena_sv_to_cstr(v->arena, n->function.decl->identifier.name);

    bool isVararg = n->type->function.variadic;
    bool confirmReturnState = validate_return_state(v, n->function.body);
    LLVMTypeRef returnType = get_return_type(n->function.body);
    // resolve_typekind(n->type->function.return_type->kind);

    LLVMTypeRef functionType =
        LLVMFunctionType(returnType, NULL, 0, (int)isVararg);

    LLVMValueRef function = LLVMAddFunction(v->module, name, functionType);

    LLVMBasicBlockRef entry = LLVMAppendBasicBlock(function, "entry");

    LLVMPositionBuilderAtEnd(v->builder, entry);

    if (n->function.body != NULL) {
      visit_stmt(v, n->function.body);
    }

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
    SomeValueRef value = some(NULL);

    if (!expr.nil) {
      value = visit_expr(v, expr.ptr);
      LLVMBuildRet(v->builder, value.ptr);
    } else
      LLVMBuildRetVoid(v->builder);

    break;

  default:
    break;
  }
}
