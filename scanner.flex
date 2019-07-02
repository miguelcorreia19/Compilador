%{
// HEADERS
#include <stdlib.h>
#include "parser.h"

// variables maintained by the lexical analyser
int yyline = 1;
%}

%option noyywrap

%%
#.*\n { yyline++;}

#.* { yyline++;}

[\n] { yyline++; }

[ \t]+ { }


"if"	 { return IFF; }
"elseif" { return ELSEIF; }
"else" 	 { return ELSEE; }
"end"    { return END; }
"while"  { return WHILEE; };
"for"    { return FORR;}
"true" 	 { return TRUE; }
"false"  { return FALSE; }
"+" 	 { return PLUS; }
"-" 	 { return MINUS; }
"*" 	 { return MULT; }
"/" 	 { return DIV; }
"%"  	 { return MOD; }
"=" 	 { return ATRIB; }
"==" 	 { return EQUAL; }
">" 	 { return MORE; }
"<" 	 { return LESS; }
">=" 	 { return MOREeq; }
"<=" 	 { return LESSeq; }
"~=" 	 { return NEQUAL; }
";" 	 { return F; }
":"		 { return COLON; }
"(" 	 { return OPENb; }
")" 	 { return CLOSEb; }
"disp" 	 { return OUTPUT; }
"input"	 { return INPUT; }

[0-9]+ { 
   yylval.intValue = atoi(yytext);
   return INT; 
}

[a-z,A-Z]+ {
	yylval.varval = strdup(yytext);
	return VAR;
}


.  { yyerror("unexpected character"); }
%%

