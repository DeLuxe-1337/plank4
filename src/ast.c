#include "ast.h"

#pragma once

#include "ast.h"
#include <stdio.h>

static void ast_print_indent(int depth) {
  for (int i = 0; i < depth; ++i)
    printf("    ");
}

static const char *ast_kind_name(AstKind kind) {
  switch (kind) {
  case AST_TRANSLATION_UNIT:
    return "TranslationUnit";
  case AST_FUNCTION:
    return "Function";
  case AST_VAR_DECL:
    return "VarDecl";
  case AST_PARAM:
    return "Param";
  case AST_TYPEDEF:
    return "Typedef";

  case AST_BLOCK:
    return "Block";
  case AST_IF:
    return "If";
  case AST_SWITCH:
    return "Switch";
  case AST_CASE:
    return "Case";
  case AST_WHILE:
    return "While";
  case AST_DO_WHILE:
    return "DoWhile";
  case AST_FOR:
    return "For";
  case AST_BREAK:
    return "Break";
  case AST_CONTINUE:
    return "Continue";
  case AST_RETURN:
    return "Return";
  case AST_GOTO:
    return "Goto";
  case AST_LABEL:
    return "Label";

  case AST_BINARY:
    return "Binary";
  case AST_UNARY:
    return "Unary";
  case AST_CALL:
    return "Call";
  case AST_MEMBER:
    return "Member";
  case AST_INDEX:
    return "Index";
  case AST_CAST:
    return "Cast";
  case AST_CONDITIONAL:
    return "Conditional";
  case AST_ASSIGN:
    return "Assign";
  case AST_COMPOUND_ASSIGN:
    return "CompoundAssign";
  case AST_COMMA:
    return "Comma";
  case AST_SIZEOF_EXPR:
    return "SizeofExpr";
  case AST_SIZEOF_TYPE:
    return "SizeofType";

  case AST_IDENTIFIER:
    return "Identifier";
  case AST_INTEGER:
    return "Integer";
  case AST_FLOAT:
    return "Float";
  case AST_STRING:
    return "String";
  case AST_CHAR:
    return "Char";

  case AST_INIT_LIST:
    return "InitList";
  case AST_EMPTY:
    return "Empty";
  }

  return "Unknown";
}

void ast_print(Ast *node, int depth) {
  if (!node) {
    ast_print_indent(depth);
    puts("(null)");
    return;
  }

  ast_print_indent(depth);

  switch (node->kind) {

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
