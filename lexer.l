%option noyywrap

%{
#include "shell.h"
char *command[MAX_ARG_COUNT];
int argcount;
%}

WORD	   [a-zA-Z0-9\/\.-]+
SPECIAL    [;*]

%%

    command[0] = NULL;
    argcount = 0;

exit|quit  { exit(0); }

cd|chdir   { printf("cd yeet time\n"); }

{WORD}|{SPECIAL} {
  if(argcount < MAX_ARG_COUNT - 1) {
    command[argcount++] = strdup(yytext);
    command[argcount] = NULL;
  }
}

">"   { printf("output direct\n"); }

"<"   { printf("input direct\n"); }

">>"  { printf("append\n"); }

"&"   { printf("ampersand\n"); }

"|"   { printf("pipe\n"); }

"("   { printf("left paren\n"); }

")"   { printf("right paren\n"); }

[ \t]+  ;

\n    { return do_command(command); }

%%

int get_command() {
  return yylex();
}
