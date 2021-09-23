%option noyywrap

%{
#include "shell.h"
char *command[MAX_ARG_COUNT];
int argcount = 0;
%}

WORD	   [a-zA-Z0-9\/\.-]+
SPECIAL    [;*]

%%

  command[0] = NULL;
  argcount = 0;

{WORD}|{SPECIAL} {
  if(argcount < MAX_ARG_COUNT - 1) {
    command[argcount++] = strdup(yytext);
    command[argcount] = NULL;
  }
}

">"   { printf("output direct at position %d\n", argcount); }

"<"   { printf("input direct\n"); }

">>"  { printf("append\n"); }

"&"   { printf("amp\n"); }

"|"   { printf("pipe\n"); }

"("   { printf("left paren\n"); }

")"   { printf("right paren\n"); }

[ \t]+  ;

\n { return do_command(command); }

%%

int get_command() {
  return yylex();
}
