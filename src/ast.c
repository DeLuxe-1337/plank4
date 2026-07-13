#include "ast.h"
#include "type.h"
#include "vector.h"

#pragma once

#include "ast.h"
#include <stdio.h>

void ast_print(Ast *node, int depth) {
  if (!node) {
    ast_print_indent(depth);
    puts("(null)");
    return;
  }

  ast_print_indent(depth);

  switch (node->kind) {

  case AST_RETURN:
    printf("Return\n");
    if (!node->ret.value.nil)
      ast_print(node->ret.value.ptr, depth + 1);
    return;

  case AST_IDENTIFIER:
    printf("Identifier(%.*s)\n", (int)node->identifier.name.len,
           node->identifier.name.data);
    return;

  case AST_INTEGER:
    printf("Integer(%llu)\n", (unsigned long long)node->integer.value);
    return;

  case AST_BINARY:
    printf("Binary(%s)\n", token_kind_name(node->binary.op.kind));

    ast_print(node->binary.lhs, depth + 1);
    ast_print(node->binary.rhs, depth + 1);
    return;

  case AST_UNARY:
    printf("Unary(%s)\n", token_kind_name(node->unary.op.kind));

    ast_print(node->unary.expr, depth + 1);
    return;

  case AST_CALL:
    puts("Call");

    ast_print(node->call.callee, depth + 1);

    for (size_t i = 0; i < node->call.args.len; ++i)
      ast_print(vector_get(Ast *, &node->call.args, i), depth + 1);

    return;

  case AST_BLOCK:
    puts("Block");

    for (size_t i = 0; i < node->block.statements.len; ++i)
      ast_print(vector_get(Ast *, &node->block.statements, i), depth + 1);

    return;

  case AST_TRANSLATION_UNIT:
    puts("TranslationUnit");

    for (size_t i = 0; i < node->translation_unit.items.len; ++i)
      ast_print(vector_get(Ast *, &node->translation_unit.items, i), depth + 1);

    return;

  case AST_FUNCTION:
    puts("Function");

    ast_print(node->function.decl, depth + 1);
    ast_print(node->function.body, depth + 1);
    return;

  case AST_IF:
    puts("If");

    ast_print(node->if_stmt.cond, depth + 1);
    ast_print(node->if_stmt.then_stmt, depth + 1);

    if (node->if_stmt.else_stmt)
      ast_print(node->if_stmt.else_stmt, depth + 1);

    return;

  default:
    printf("%s\n", ast_kind_name(node->kind));
    return;
  }
}
Ast *ast_new(Arena *arena, AstKind kind) {
  Ast *node = arena_new_zero(arena, Ast);
  node->kind = kind;
  return node;
}
Ast *ast_identifier(Arena *arena, StringView name) {
  Ast *n = ast_new(arena, AST_IDENTIFIER);

  n->identifier.name = name;

  return n;
}
Ast *ast_integer(Arena *arena, uint64_t value) {
  Ast *n = ast_new(arena, AST_INTEGER);

  n->integer.value = value;

  return n;
}
Ast *ast_binary(Arena *arena, Token op, Ast *lhs, Ast *rhs) {
  Ast *n = ast_new(arena, AST_BINARY);

  n->binary.op = op;
  n->binary.lhs = lhs;
  n->binary.rhs = rhs;

  return n;
}
Ast *ast_function(Arena *arena, Ast *decl, Ast *body) {
  Ast *n = ast_new(arena, AST_FUNCTION);
  n->type = type_new(arena, TYPE_FUNCTION);
  n->type->function.return_type = type_new(arena, TYPE_VOID);
  n->type->function.variadic = false;

  n->function.decl = decl;
  n->function.body = body;
  return n;
}
Ast *ast_block(Arena *arena) {
  Ast *n = ast_new(arena, AST_BLOCK);
  n->block.statements = vector_init(Ast *);

  return n;
}
Ast *ast_return(Arena *arena, Some value) {
  Ast *n = ast_new(arena, AST_RETURN);

  n->ret.value = value;

  return n;
}