// AST definitions
#ifndef __ast_h__
#define __ast_h__

// AST for expressions
typedef struct _Expr 
{
  enum 
  { 
    E_INTEGER,
    E_VARIABLE,
    E_OPERATION,
    E_OPBOOLEAN,
    E_BRACKETS,
    E_INPUT
  } kind;

  union 
  {
    int value; // E_INTEGER
    
    char *var; // E_VARIABLE

    struct // E_OPERATION
    { 
      int operator; // PLUS, MINUS, etc 
      struct _Expr* left;
      struct _Expr* right;
    } op;

    struct _Expr* exp; // E_BRACKETS 

  } attr;
}* Expr;

typedef struct _cmd
{
  enum
  {
    E_ATRIB,
    E_IF,
    E_WHILE,
    E_FOR, 
    E_OUTPUT//,
    //E_INPUT

  } kind;
  
  union
  {
    struct // E_ATRIB
    {
      char var[200];
      Expr exp;
    } atrib;

    struct // E_IF -> if elseif(LIST) else
    {
      Expr cond;
      struct _cmdList* body;

      struct _elseifList* elseiff;      

      struct _cmdList* elsee;
    } iff;

    struct // E_WHILE
    {
      Expr cond;
      struct _cmdList* body;
    } whilee;

    struct // E_FOR
    {
      struct _cmd* decl;
      Expr cond;
      struct _cmdList* body;
    } forr;

    Expr output; // E_OUTPUT
  } attr;
}* cmd;


typedef struct _cmdList
{
  cmd head;
  struct _cmdList *tail;
}* cmdList;

typedef struct _elseif
{
  Expr cond;
  cmdList body;
}* elseif;

typedef struct _elseifList
{
  elseif head;
  struct _elseifList *tail;
}* elseifList;


Expr ast_integer(int v);
Expr ast_var(char *v);
Expr ast_operation(int operator, Expr left, Expr right);
Expr ast_boolean(int operator, Expr left, Expr right);
Expr ast_brackets(Expr exp);
Expr ast_input();


cmdList mklist(cmd head, cmdList tail);
elseifList mkElseIflist(elseif head, elseifList tail);
cmd mkAtrib(char *var, Expr e);
cmd mkIf(Expr ifCond, cmdList ifBody, elseifList ElseIFlist, cmdList elsee);
elseif mkElseIf(Expr cond, cmdList body);
cmd mkWhile(Expr cond, cmdList body);
cmd mkFor(cmd decl, Expr cond, cmdList body);
cmd mkOutput(Expr e);
cmd mkInput(char *var);

#endif
