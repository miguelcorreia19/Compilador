#include <string.h> 
#include <stdio.h>

#include <stdlib.h> // for malloc
#include "code.h" // AST header

Address C_mkInt(int i)
{
  Address node = (Address) malloc(sizeof(struct _Address));
  node->kind = C_INT;
  node->content.val = i;
  return node;
}

Address C_mkChar(char *v)
{
  Address node = (Address) malloc(sizeof(struct _Address));
  node->kind = C_VAR;
  node->content.var = v;
  return node;
}

Instr C_mkInstr(OpKind op, Address a1, Address a2, Address a3)
{
  Instr node = (Instr) malloc(sizeof(struct _Instr));
  node->op = op;
  node->addr1 = a1;
  node->addr2 = a2;
  node->addr3 = a3;
  return node;  
}

InstrList C_mkInstrList(Instr head, InstrList tail)
{
  InstrList node = (InstrList) malloc(sizeof(struct _InstrList));

  node->inst = head;
  node->next = tail;
  return node;
}

Pair C_mkPair(Address a, InstrList i)
{
  Pair node = (Pair) malloc(sizeof(struct _Pair));

  node->name = a;
  node->list = i;
  return node;
}

Stack C_mkStack(char * v, Stack st)
{
  Stack node = (Stack) malloc(sizeof(struct _Stack));

  node->var = v;
  node->next = st;
  return node;
}

int C_mkStackContains(char * v, Stack st)
{
  if(st == NULL)
    return 0;

  if(strcmp(v,st->var) == 0)
  {
    return 1;
  }

  while(st != NULL)
    {
      if(strcmp(v,st->var) == 0)
        return 1;
      st = st->next;
    }
    return 0;
}

// LISTS && INSTR
Instr head(InstrList l)
{
  if(l != NULL)
    return(l->inst);
}

InstrList tail(InstrList l)
{
  if(l != NULL)
    return(l->next); 
}

Instr last(InstrList l)
{
  while(tail(l) != NULL)
    l = tail(l);
  return l->inst;
}

InstrList removeLast(InstrList list)
{
  InstrList p = list;
  while(p->next->next != NULL)
  {
    p = p->next;
  }
  p->next = NULL;
  
  return list;
}

InstrList append(InstrList l1, InstrList l2)
{
  InstrList p;

  if(l1 == NULL)
      return l2;
  if(l2 == NULL)
      return l1;

  for(p = l1; tail(p) != NULL; p = tail(p));

  //p = tail(p); // ERROR não percebo porque...
  p->next = l2;
  return l1;
}
