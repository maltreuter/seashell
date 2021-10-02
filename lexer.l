%option noyywrap
%option yylineno

%{
//#define YYSTYPE char *
#include "shell.h"
#include "y.tab.h"
extern YYSTYPE yylval;
%}

WORD	   [a-zA-Z0-9\/\.-]+
SPECIAL    [;*]

%%

exit|quit  { exit(0); }

cd|chdir   { printf("cd yeet time\n"); }

{WORD}|{SPECIAL} {
  yylval.string = strdup(yytext);
  return WORD;
}

">"   { printf("output direct\n"); }

"<"   { printf("input direct\n"); }

">>"  { printf("append\n"); }

"&"   { printf("ampersand\n"); }

"|"   { printf("pipe\n"); }

"("   { printf("left paren\n"); }

")"   { printf("right paren\n"); }

[ \t]+  ;

\n    { return TERM; }

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
