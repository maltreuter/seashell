%option noyywrap

%{
#include "shell.h"
char *command[MAX_ARG_COUNT];
int argcount;
%}

WORD	   [a-zA-Z0-9\/\.-]+
SPECIAL    [><&|();*]

%%

    command[0] = NULL;
    argcount = 0;

exit|quit  { exit(0); }

{WORD}|{SPECIAL} {
  if(argcount < MAX_ARG_COUNT - 1) {
    command[argcount++] = strdup(yytext);
    command[argcount] = NULL;
  }
}

[ \t]+  ;

\n    { return do_command(command, 0, 0, 0); }

%%

int get_command() {
  return yylex();
}
