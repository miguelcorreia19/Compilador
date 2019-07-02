#include <stdio.h>
#include "parser.h"
#include "code.h"
FILE *f;

#include "printer.h"
#include "compiler.h"

InstrList listToPrint = NULL;

int main(int argc, char** argv) 
{
  --argc; ++argv;
  if (argc != 0) 
  {
    yyin = fopen(*argv, "r");
    if (!yyin) 
    {
      printf("'%s': could not open file\n", *argv);
      return 1;
    }
  } //  yyin = stdin

  if (yyparse() == 0) 
  {
    f = fopen("mips.txt", "w");

    while(root != NULL)
    {
      countVar = 0;
      listToPrint = append(listToPrint, Commands(root->head));
      Instr inst = C_mkInstr(C_SPACE, NULL, NULL, NULL);
      listToPrint = append(listToPrint, C_mkInstrList(inst, NULL));
      root = root->tail;
    }

    printStack(stack);
    printInstrList(listToPrint);
    printExit();

    fclose(f);
  }

  return 0;
}
