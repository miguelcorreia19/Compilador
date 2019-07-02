// Tokens
%token 
  INT  		// 1,2,3 ....
  PLUS 		// +
  MINUS		// -
  MULT		// *
  DIV		  // /
  MOD		  // %
  BRACKETS// ( Expr )
  F			  // ;
  COLON		// :
  VAR 		// x, i ....
  ATRIB 	// =
  EQUAL		// ==
  NEQUAL  // ~=
  MORE		// >
  LESS		// <
  MOREeq	// >=
  LESSeq	// <=
  IFF		  // if expr cmd_list
  ELSEIF 	// elseif expr cmd_list
  ELSEE		// else cmd_list
  END		
  WHILEE	// while expr cmd_list
  FORR		// for cmd:expr cmd_list
  TRUE		// 1
  FALSE		// 0
  OPENb		// (
  CLOSEb	// )
  INPUT   // input
  OUTPUT  // disp

%right EQUAL NEQUAL MORE LESS MOREeq LESSeq
%left PLUS MINUS
%left DIV MULT
%precedence NEG

// Root-level grammar symbol
%start program;


// Types/values in association to grammar symbols.
%union 
{
  int intValue;
  Expr exprValue;                
  char *varval;
  cmdList cmd_l; 
  elseifList elseif_l;
  elseif elseif_;
  cmd cmd_;
}

%type <intValue> INT

%type <exprValue> exprB // Exp bool
%type <exprValue> expr  // Op

%type <varval> VAR

%type <cmd_> for_atrib
%type <cmd_> cmd
%type <cmd_l> cmd_list

%type <elseif_l> cmd_elseif_list
%type <elseif_> cmd_elseif

// Use "%code requires" to make declarations go
// into both parser.c and parser.h
%code requires 
{
  #include <stdio.h>
  #include <stdlib.h>
  #include "ast.h"
  #include "code.h"

  extern int yylex();
  extern int yyline;
  extern char* yytext;
  extern FILE* yyin;
  extern void yyerror(const char* msg);
  cmdList root;
}

%%
program: cmd_list { root = $1; }                         


cmd_list: 

  	  cmd cmd_list     { $$ = mklist($1,$2); }

  	| cmd           { $$ = mklist($1,NULL); }

cmd_elseif_list: 

      cmd_elseif cmd_elseif_list     { $$ = mkElseIflist($1,$2); }

    | cmd_elseif                     { $$ = mkElseIflist($1,NULL); }

cmd_elseif:

    ELSEIF exprB cmd_list { $$ = mkElseIf($2, $3); }

cmd:

	  VAR ATRIB expr F                                              { $$ = mkAtrib($1,$3); } 

  | VAR ATRIB exprB F                                              { $$ = mkAtrib($1,$3); } 

  | IFF exprB cmd_list END                                         { $$ = mkIf($2, $3, NULL, NULL); }

  | IFF exprB cmd_list cmd_elseif_list END                         { $$ = mkIf($2, $3, $4, NULL); }

  | IFF exprB cmd_list cmd_elseif_list ELSEE cmd_list END          { $$ = mkIf($2, $3, $4, $6); }

  | IFF exprB cmd_list ELSEE cmd_list END                          { $$ = mkIf($2,$3,NULL, $5); }

  | WHILEE exprB cmd_list END                                      { $$ = mkWhile($2, $3); }

  | FORR for_atrib COLON expr cmd_list END                         {$$ = mkFor($2, $4, $5); }

  | OUTPUT OPENb expr CLOSEb F                                     { $$ = mkOutput($3); }


for_atrib:
  
  VAR ATRIB expr     { $$ = mkAtrib($1,$3); }

expr:
	
    INT                { $$ = ast_integer($1); } 

  | INPUT OPENb CLOSEb { $$ = ast_input(); }

  | OPENb expr CLOSEb  { $$ = ast_brackets($2); }

  | MINUS INT %prec NEG { $$ = ast_integer(-$2); }

	| TRUE					     { $$ = ast_integer(1); }  

	| FALSE				  	   { $$ = ast_integer(0); }  

 	| VAR 					     { $$ = ast_var($1); }

 	| expr PLUS expr     { $$ = ast_operation(PLUS, $1, $3); }

 	| expr MINUS expr    { $$ = ast_operation(MINUS, $1, $3); } 
  
 	| expr MULT expr     { $$ = ast_operation(MULT, $1, $3); }

 	| expr DIV expr 	   { $$ = ast_operation(DIV, $1, $3); }

 	| expr MOD expr 	   { $$ = ast_operation(MOD, $1, $3); }

  | expr EQUAL expr  { $$ = ast_operation(EQUAL, $1, $3); }

  | expr NEQUAL expr { $$ = ast_operation(NEQUAL, $1, $3); }

  | expr MORE expr   { $$ = ast_operation(MORE, $1, $3); }

  | expr LESS expr   { $$ = ast_operation(LESS, $1, $3); }

  | expr MOREeq expr { $$ = ast_operation(MOREeq, $1, $3); }

  | expr LESSeq expr { $$ = ast_operation(LESSeq, $1, $3); }

exprB:

  INT                { $$ = ast_boolean(NEQUAL, ast_integer($1), ast_integer(0)); } 

  | INPUT OPENb CLOSEb { $$ = ast_boolean(NEQUAL, ast_input(), ast_integer(0)); }

  | MINUS INT %prec NEG { $$ = ast_boolean(NEQUAL, ast_integer(-$2), ast_integer(0)); }

  | TRUE               { $$ = ast_boolean(NEQUAL, ast_integer(1), ast_integer(0)); }  

  | FALSE              { $$ = ast_boolean(NEQUAL, ast_integer(0), ast_integer(0)); }  

  | VAR                { $$ = ast_boolean(NEQUAL, ast_var($1), ast_integer(0)); }

  | expr EQUAL expr  { $$ = ast_boolean(EQUAL, $1, $3); }

  | expr NEQUAL expr { $$ = ast_boolean(NEQUAL, $1, $3); }

 	| expr MORE expr 	 { $$ = ast_boolean(MORE, $1, $3); }

 	| expr LESS expr 	 { $$ = ast_boolean(LESS, $1, $3); }

 	| expr MOREeq expr { $$ = ast_boolean(MOREeq, $1, $3); }

 	| expr LESSeq expr { $$ = ast_boolean(LESSeq, $1, $3); };


%%

void yyerror(const char* err) 
{
    printf("Line %d: %s - '%s'\n", yyline, err, yytext);
}

//evallist function

