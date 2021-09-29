%{
#include <stdio.h>
#include <string.h>

#define YYSTYPE char *

int yylex();
int yyparse();
void yyerror(const char *s);
%}

%token COM PIPE AMP

%%

commands:   /* empty */
        |   commands command
        ;

command:
        COM
        {
            printf("%s\n", $1);
        }
        ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "error: %s\n", s);
}

int main(void) {
    yyparse();
}
