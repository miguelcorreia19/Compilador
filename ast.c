// AST constructor functions
#include <string.h> 
#include <stdlib.h> // for malloc
#include "ast.h" // AST header

Expr ast_integer(int v) 
{
  Expr node = (Expr) malloc(sizeof(struct _Expr));
  node->kind = E_INTEGER;
  node->attr.value = v;
  return node;
}

Expr ast_operation (int operator, Expr left, Expr right) 
{
  Expr node = (Expr) malloc(sizeof(struct _Expr));
  node->kind = E_OPERATION;
  node->attr.op.operator = operator;
  node->attr.op.left = left;
  node->attr.op.right = right;
  return node;
}

Expr ast_boolean (int operator, Expr left, Expr right) 
{
  Expr node = (Expr) malloc(sizeof(struct _Expr));
  node->kind = E_OPBOOLEAN;
  node->attr.op/*_bl*/.operator = operator;
  node->attr.op/*_bl*/.left = left;
  node->attr.op/*_bl*/.right = right;
  return node;
}

Expr ast_brackets(Expr exp)
{
  Expr node = (Expr) malloc(sizeof(struct _Expr));
  node->kind = E_BRACKETS;
  node->attr.exp = exp;
  return node;
}

Expr ast_input()
{
  Expr node = (Expr) malloc(sizeof(struct _Expr));
  node->kind = E_INPUT;
  return node;
}

Expr ast_var(char *v)
{
	Expr node = (Expr) malloc(sizeof(struct _Expr));
  node->kind = E_VARIABLE;
  node->attr.var = v;
  return node;
}

cmdList mklist(cmd head, cmdList tail)
{
	cmdList node = (cmdList) malloc(sizeof(struct _cmdList));
  node->head = head;
  node->tail = tail;
  return node;	
}

elseifList mkElseIflist(elseif head, elseifList tail)
{
  elseifList node = (elseifList) malloc(sizeof(struct _elseifList));
  node->head = head;
  node->tail = tail;
  return node;  
}

cmd mkAtrib(char *var, Expr e)
{
	cmd node = (cmd) malloc(sizeof(struct _cmd));
  node->kind = E_ATRIB;
  strcpy(node->attr.atrib.var,var);
  node->attr.atrib.exp = e;
  return node;		
}

cmd mkIf(Expr ifCond, cmdList ifBody, elseifList ElseIFlist, cmdList elsee)
{
  cmd node = (cmd) malloc(sizeof(struct _cmd));
  node->kind = E_IF;
  node->attr.iff.cond = ifCond;
  node->attr.iff.body = ifBody;

  node->attr.iff.elseiff = ElseIFlist;
  node->attr.iff.elsee = elsee;
  return node;     
}

elseif mkElseIf(Expr cond, cmdList body)
{
  elseif node = (elseif) malloc(sizeof(struct _elseif));
  node->cond = cond;
  node->body = body;
  return node;     
}

cmd mkWhile(Expr cond, cmdList body)
{
  cmd node = (cmd) malloc(sizeof(struct _cmd));
  node->kind = E_WHILE;
  node->attr.whilee.cond = cond;
  node->attr.whilee.body = body;
  return node;     
}

cmd mkFor(cmd decl, Expr cond, cmdList body)
{
  cmd node = (cmd) malloc(sizeof(struct _cmd));
  node->kind = E_FOR;
  node->attr.forr.decl = decl; 
  node->attr.forr.cond = cond;
  node->attr.forr.body = body;
  return node;     
}

cmd mkOutput(Expr e)
{
  cmd node = (cmd) malloc(sizeof(struct _cmd));
  node->kind = E_OUTPUT;
  node->attr.output = e; 
  return node;      
}