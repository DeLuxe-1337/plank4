#pragma once
#include "ast.h"
#ifndef VISITOR_H
#define VISITOR_H

void visit_expr(Ast *n);

void visit_stmt(Ast *n)
{
    switch (n->kind) {

    case AST_IF:
        visit_expr(n->if_stmt.cond);
        visit_stmt(n->if_stmt.then_stmt);

        if (n->if_stmt.else_stmt)
            visit_stmt(n->if_stmt.else_stmt);
        break;

    case AST_BLOCK:
        // for (...)
        //     visit_stmt(...);
        break;

    case AST_RETURN:
        // visit_expr(...);
        break;

    default:
        break;
    }
}

#endif