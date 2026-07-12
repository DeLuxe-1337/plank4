#include "arena.h"
#include "ast.h"
#include "tokenizer.h"
#include <llvm-c/Core.h>
#include <llvm-c/Target.h>
#include <llvm-c/TargetMachine.h>
#include <stdio.h>

int main(void) {
  printf("Hello, world\n");
  // const char *source = "function main {}";

  // Lexer lex = {
  //     .start = source,
  //     .current = source,
  //     .line = 1,
  //     .column = 1,
  // };

  // Arena arena;
  // arena_init(&arena, sizeof(Ast));

  // Token plus = make_token(&lex, TOK_PLUS, NULL, 0, 0);

  // Ast *expr = ast_binary(&arena, plus, ast_identifier(&arena, SV("x")),
  //                        ast_integer(&arena, 5));
  // printf("Hello, world\n");

  // ast_print(expr, 0);

  // Token tok;

  // do {
  //   tok = lexer_next(&lex);

  //   printf("%-20s '%.*s' (%u:%u)\n", token_kind_name(tok.kind),
  //   (int)tok.length,
  //          tok.begin, tok.line, tok.column);

  // } while (tok.kind != TOK_EOF);

  // arena_destroy(&arena);

  LLVMModuleRef module = LLVMModuleCreateWithName("my_module");

  LLVMTypeRef i32 = LLVMInt32Type();
  LLVMTypeRef fn_type = LLVMFunctionType(i32, NULL, 0, 0);
  LLVMValueRef fn = LLVMAddFunction(module, "main", fn_type);

  LLVMBasicBlockRef entry = LLVMAppendBasicBlock(fn, "entry");

  LLVMBuilderRef builder = LLVMCreateBuilder();
  LLVMPositionBuilderAtEnd(builder, entry);

  LLVMBuildRet(builder, LLVMConstInt(i32, 0, 0));

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
  if (LLVMTargetMachineEmitToFile(machine, module, "main.s", LLVMAssemblyFile,
                                  &error)) {
    fprintf(stderr, "%s\n", error);
    return 1;
  }

  LLVMDisposeMessage(triple);
  LLVMDisposeBuilder(builder);
  LLVMDisposeModule(module);
  LLVMDisposeTargetMachine(machine);

  return 0;
}