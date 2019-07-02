#include <stdio.h>
#include "parser.h"

int countCmd = 0;

void putTabs(int tab)
{
  int i = 0;
  for(i = 0; i < tab; i++)
  {
    printf(" ");
  }
}

void printExpr(Expr expr) 
{
  if (expr->kind == E_INTEGER) 
  {
    printf(" %d ", expr->attr.value); 
  } 

  else if (expr->kind == E_OPERATION) 
  {
    if(expr->attr.op.operator == BRACKETS)
    {
      printf(" (");
      printExpr(expr->attr.op.left); // Lado direito está NULL (BISON)
      printf(") ");
    }
    else
    {
      printExpr(expr->attr.op.left);
         
      switch (expr->attr.op.operator) 
      {
        case PLUS: 
          printf(" + ");  
          break;

        case MINUS: 
          printf(" - ");
          break;

        case DIV: 
          printf(" / ");
          break;
        
        case MULT: 
          printf(" * ");
          break;
      
        case MOD: 
          printf(" %% ");
          break;
      }

      printExpr(expr->attr.op.right);
    }
  }

  else if (expr->kind == E_OPBOOLEAN) 
  {
    printExpr(expr->attr.op_bl.left);
    
    switch (expr->attr.op_bl.operator) 
    {
      case EQUAL: 
        printf(" == ");
        break;

      case NEQUAL: 
        printf(" ~= ");
        break;

      case MORE: 
        printf(" > ");
        break;

      case LESS: 
        printf(" < ");
        break;

      case MOREeq: 
        printf(" >= ");
        break;

      case LESSeq: 
        printf(" <= ");
        break;
    }

    printExpr(expr->attr.op_bl.right);
  }

  else if (expr->kind == E_VARIABLE) 
  {
    printf(" %s ", expr->attr.var);
  }
}

void printCmd(cmd command, int tab) 
{
  countCmd++;
  
  putTabs(tab);

  if (command->kind == E_ATRIB) 
  {
    printf("%s =", command->attr.atrib.var); 
    printExpr(command->attr.atrib.exp);
    printf("\n");
  } 

  else if(command->kind == E_IF)
  {
    printf("IF ");
    printExpr(command->attr.iff.cond); 

    printf("\n");
    putTabs(tab);
    printf("{");

    cmdList command2 = command->attr.iff.body; 

    while(command2!=NULL)
    { 
      printf("\n");
      printCmd(command2->head, tab+2);
      command2 = command2->tail;
    }

    putTabs(tab);
    printf("}\n");

    elseifList list = command->attr.iff.elseiff; 

    while(list!=NULL)
    { 
      printf("\n");

      putTabs(tab);
      printf("ELSEIF ");
      
      elseif command_elseif = list->head;
     
      printExpr(command_elseif->cond);

      printf("\n");
      putTabs(tab);
      printf("{");
      
      command2 = command_elseif->body; 
      while(command2 != NULL)
      { 
        printf("\n");
        printCmd(command2->head, tab+2);
        command2 = command2->tail;
      }

      list = list->tail;
      putTabs(tab);
      printf("}\n");
    }
    
    if(command->attr.iff.elsee != NULL)
    {
      putTabs(tab);
      printf("ELSE");

      printf("\n"); 
      putTabs(tab);
      printf("{");
    
      command2 = command->attr.iff.elsee;
      while(command2!=NULL)
      { 
        printf("\n");
        printCmd(command2->head, tab+2);
        command2 = command2->tail;
      }

      putTabs(tab);
      printf("}\n");
    }
   // printf("\n");
  }

  else if(command->kind == E_WHILE)
  {
    printf("WHILE ");

    printExpr(command->attr.whilee.cond); 
    
    printf("\n");
    putTabs(tab);
    printf("{");

    cmdList list = command->attr.whilee.body; 
    
    while(list!=NULL)
    { 
      printf("\n");
      printCmd(list->head, tab+2);
      list = list->tail;
    }

    putTabs(tab);
    printf("}\n");
  }

  else if(command->kind == E_FOR)
  {
    printf("FOR ");

    cmd command2 =  command->attr.forr.decl;
    printf("%s =", command2->attr.atrib.var); 
    printExpr(command2->attr.atrib.exp);

    printf(":");

    printExpr(command->attr.forr.cond);   
    
    printf("\n");
    putTabs(tab);
    printf("{");
    
    cmdList list = command->attr.forr.body; 
    
    while(list!=NULL)
    { 
      printf("\n");
      printCmd(list->head, tab+2);
      list = list->tail;
    }

    putTabs(tab);
    printf("}\n");
  }

  else if(command->kind == E_OUTPUT)
  {
    printf("DISPLAY ");

    printExpr(command->attr.output);
    printf("\n");
  }  

  else if(command->kind == E_INPUT)
  {
    printf("INPUT %s", command->attr.input);
    printf("\n");
  }  
}

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
    while(root != NULL)
    {
      printCmd(root->head, 0);
      printf("\n");
      root = root->tail;
    }
  }
  return 0;
}
