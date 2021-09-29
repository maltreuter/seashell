%option noyywrap

%{
#define YYSTYPE char *
#include "shell.h"
#include "y.tab.h"
extern YYSTYPE yylval;
%}

WORD	   [a-zA-Z0-9\/\.-]+
SPECIAL    [;*]

%%

{WORD}|{SPECIAL} {
  yylval = strdup(yytext);
  return COM;
}

">"   { printf("output direct\n"); }

"<"   { printf("input direct\n"); }

">>"  { printf("append\n"); }

"&"   { return AMP; }

"|"   { return PIPE; }

"("   { printf("left paren\n"); }

")"   { printf("right paren\n"); }

[ \t]+  ;

\n  ;

%%

/*
  command[0] = NULL;
  argcount = 0;

{WORD}|{SPECIAL} {
  if(argcount < MAX_ARG_COUNT - 1) {
    command[argcount++] = strdup(yytext);
    command[argcount] = NULL;
  }
  return COM;
} */

/* int get_command() {
  return yylex();
} */
