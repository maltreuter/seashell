%option noyywrap
%option noinput
%option nounput

%{
#include "shell.h"
char *command[MAX_ARG_COUNT];
int argcount;
%}

WORD	   [a-zA-Z0-9\/\.-]+
SPECIAL    [><&|();*]+

%%

    command[0] = NULL;
    argcount = 0;

{WORD}|{SPECIAL} {
	if(argcount < MAX_ARG_COUNT - 1) {
    	command[argcount++] = strdup(yytext);
    	command[argcount] = NULL;
  	}
}

[ \t]+  ;

\n    { return set_command(command); }

%%

int get_input() {
  return yylex();
}

int stop_lex() {
	return yylex_destroy();
}
