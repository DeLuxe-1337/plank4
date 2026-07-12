#include "arena.h"
#include "ast.h"
#include "tokenizer.h"
#include "visitor.h"
#include <llvm-c/Core.h>
#include <llvm-c/Target.h>
#include <llvm-c/TargetMachine.h>
#include <stdio.h>

int main(void) {
  printf("Hello, world\n");
  const char *source = "function main {}";

  Lexer lex = {
      .start = source,
      .current = source,
      .line = 1,
      .column = 1,
  };

  Token tok;

  do {
    tok = lexer_next(&lex);

    printf("%-20s '%.*s' (%u:%u)\n", token_kind_name(tok.kind), (int)tok.length,
           tok.begin, tok.line, tok.column);

  } while (tok.kind != TOK_EOF);

  Arena arena;
  arena_init(&arena, sizeof(Ast));

  Token plus = make_token(&lex, TOK_PLUS, NULL, 0, 0);

  Ast *stmt = ast_function(&arena, ast_identifier(&arena, SV("main")), NULL);

  ast_print(stmt, 0);

  Visitor *visitor = visitor_create(&arena, "module");
  visit_stmt(visitor, stmt);

  // Initialize LLVM targets
  LLVMInitializeNativeTarget();
  LLVMInitializeNativeAsmPrinter();

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
  if (LLVMTargetMachineEmitToFile(machine, visitor->module, "main.s",
                                  LLVMAssemblyFile, &error)) {
    fprintf(stderr, "%s\n", error);
    return 1;
  }

  char *ir = LLVMPrintModuleToString(visitor->module);
  printf("%s\n", ir);

  LLVMDisposeMessage(ir);
  LLVMDisposeMessage(triple);
  LLVMDisposeTargetMachine(machine);
  visitor_cleanup(visitor);
  arena_destroy(&arena);

  return 0;
}