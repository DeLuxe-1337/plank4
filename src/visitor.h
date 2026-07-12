#pragma once
#include "ast.h"
#include "some.h"
#include <llvm-c/Core.h>
#include <llvm-c/Target.h>
#include <llvm-c/TargetMachine.h>
#ifndef VISITOR_H
#define VISITOR_H

typedef struct {
    LLVMModuleRef module;
    LLVMBuilderRef builder;
    Arena* arena;
} Visitor;

Visitor* visitor_create(Arena* arena, const char* name);
void visitor_cleanup(Visitor *v);

Some visit_expr(Visitor* visitor, Ast *n);
void visit_stmt(Visitor* visitor, Ast *n);

#endif