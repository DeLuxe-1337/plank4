#include "arena.h"
#include "ast.h"
#include "parser.h"
#include "tokenizer.h"
#include "visitor.h"
#include <llvm-c/Analysis.h>
#include <llvm-c/Core.h>
#include <llvm-c/Target.h>
#include <llvm-c/TargetMachine.h>
#include <stdio.h>

int main(void) {
  printf("Hello, world\n");
  const char *source = ""
                       //  "function foo {}\n"
                       "function main {"
                       "return 1;"
                       "}";

  Lexer lex = {
      .start = source,
      .current = source,
      .line = 1,
      .column = 1,
  };

  Arena arena;
  arena_init(&arena, sizeof(Ast));

  Parser *p = parser_create(&arena, &lex);

  Ast *stmt = NULL;

  Visitor *visitor = visitor_create(&arena, "module");

  while (!p->eof) {
    stmt = parse_stmt(p);

    if (p->eof)
      break;

    visit_stmt(visitor, stmt);
  }

  // Initialize LLVM targets

  LLVMInitializeNativeTarget();
  LLVMInitializeNativeAsmPrinter();
  LLVMInitializeNativeAsmParser();

  // Get the host target
  char *triple = LLVMGetDefaultTargetTriple();

  LLVMTargetRef target;
  char *error = NULL;

  if (LLVMGetTargetFromTriple(triple, &target, &error)) {
    fprintf(stderr, "%s\n", error);
    return 1;
  }

  // Create target machine
  LLVMTargetMachineRef machine =
      LLVMCreateTargetMachine(target, triple, "", "", LLVMCodeGenLevelDefault,
                              LLVMRelocDefault, LLVMCodeModelDefault);

  // Emit object file
  if (LLVMTargetMachineEmitToFile(machine, visitor->module, "main.o",
                                  LLVMObjectFile, &error)) {
    fprintf(stderr, "%s\n", error);
    return 1;
  }

  if (LLVMVerifyModule(visitor->module, LLVMReturnStatusAction, &error)) {
    fprintf(stderr, "%s\n", error);
  }

  char *ir = LLVMPrintModuleToString(visitor->module);
  printf("%s\n", ir);

  LLVMDisposeMessage(ir);
  LLVMDisposeMessage(triple);
  LLVMDisposeMessage(error);
  LLVMDisposeTargetMachine(machine);
  visitor_cleanup(visitor);
  arena_destroy(&arena);

  return 0;
}