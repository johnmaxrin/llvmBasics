%{

#include <stdio.h>
#include <stdlib.h>

#include "include/ast/ast.h"
#include "include/ast/codegen.h"
#include "include/ast/visitor.h"

typedef struct yy_buffer_state * YY_BUFFER_STATE;
extern YY_BUFFER_STATE yy_scan_string(const char * str); 

void yyerror(const char *s);
int yylex(void);

extern int yyparse();

extern Expression* root;

%}

%union {
    int intval;
    Expression *expr;
}

%token <intval> NUMBER
%token PLUS MINUS
%type <expr> expression

%%

program:
    expression
    {
        root = $1;
    }
    ;

expression:
    expression PLUS expression  { $$ = new Expression('+', $1, $3); }
    | expression MINUS expression { $$ = new Expression('-', $1, $3); }
    | NUMBER { $$ = new Expression($1); }
    ;

%%

void yyerror(const char *s)
{
    fprintf(stderr, "error: %s\n", s);
}

