#include "./src/pacellvm.h"
#include "include/ast/ast.h"
#include "include/ast/codegen.h"
#include "pacelang.tab.h"

extern int yyparse();
extern void yy_scan_string(const char* str);

extern Expression* root;


int main(int argc, char *argv[]){

    root = nullptr;

    if (argc > 1) {
        yy_scan_string(argv[1]);
    } else {
        fprintf(stderr, "Please provide an input string.\n");
        return 1;
    }
    yyparse();

    if(root)
    {
        //printf("Hello");

        Codegen *gen = new Codegen;
        
        root->Accept(gen);
        gen->generateCode("pacelang.ll");
    }
    
    return 0;
}




