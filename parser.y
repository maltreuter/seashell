%{
#include <stdio.h>
#include <string.h>
#include "shell.h"

extern int yylineno;

int yylex();
int yyparse();
void yyerror(const char *s);
%}

%token TERM

%token <string> WORD
%token <args_n> ARGS

%type <args_n> arg_list

%initial-action
{
    printf("->");
}

%union {
    char *string;
    struct args_n *head;
}

%%

commands:   /* empty */
        |   commands error TERM { yyerrok; }
        |   commands arg_list TERM { printf("commands arg_list TERM\n"); }
        ;
arg_list:
        WORD arg_list { printf("WORD arg_list: %s\n", $1); }
        |
        ARGS arg_list { printf("ARGS arg_list\n"); }
        |
        ARGS { printf("ARGS\n"); }
        |
        WORD { printf("WORD: %s\n", $1); }
        ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "error at line %d: %s\n", yylineno, s);
}

int main(void) {
    //printf("->");
    yyparse();
}
