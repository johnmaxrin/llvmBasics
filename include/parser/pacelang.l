%{
#include <stdio.h>
#include "include/ast/ast.h"
#include "pacelang.tab.h"

// External declarations for parser and scanner
extern int yylex();
extern int yyparse();
extern void yyerror(const char *s);

%}


%%
[0-9]+      { yylval.intval = atoi(yytext); return NUMBER;}
"+"         { return PLUS; }
"-"         { return MINUS; }
[ \t\n]+    { /* skip whitespace */ }
.           {return yytext[0];}
%%

int yywrap() {
    return 1;
}

