#ifndef __code_h__
#define __code_h__

typedef enum
{
  C_ADD,
  C_SUB,
  C_DIV,
  C_MUL,
  C_BR,

  C_BEQ, // ==
  C_BNE, // !=
  C_BLE, // <=
  C_BGE, // >=
  C_BLT, // <
  C_BGT, // >

  C_SEQ, // ==
  C_SNE, // !=
  C_SLE, // <=
  C_SGE, // >=
  C_SLT, // <
  C_SGT, // >

  C_LW,
  C_LI,
  C_SW,
  C_MOVE,
  C_J,
  
  C_SYSCALL,
  C_LABEL,
  C_SPACE

} OpKind; 

typedef enum
{
  C_INT,
  C_VAR,
} AddrKind; 

typedef struct _Address
{
  AddrKind kind;

  union
  {
    int val;
    char *var;
  } content;
  
} *Address;

typedef struct _Instr
{
  OpKind op;
  Address addr1, addr2, addr3;
} *Instr;


typedef struct _InstrList
{
	Instr inst;
 	struct _InstrList *next;
} *InstrList;

typedef struct _Pair
{
	Address name;
	InstrList list;
} *Pair;

typedef struct _Stack
{
  char *var;
  struct _Stack *next;
} *Stack;

Address C_mkInt(int i);
Address C_mkChar(char *v);

Instr C_mkInstr(OpKind op, Address a1, Address a2, Address a3);

InstrList C_mkInstrList(Instr head, InstrList tail);

Pair C_mkPair(Address a, InstrList inst);

InstrList append(InstrList l1, InstrList l2);
InstrList tail(InstrList l);
Instr last(InstrList l);
InstrList removeLast(InstrList list);

Stack C_mkStack(char * v, Stack st);
int C_mkStackContains(char * v, Stack st);

#endif