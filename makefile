CC=gcc
FLAGS=-g -Wall

all: lex shell

lex: shell_lexer.l
	flex shell_lexer.l

shell: shell.c lex.yy.c
	$(CC) -o shell shell.c lex.yy.c $(FLAGS)

clean:
	rm lex.yy.c shell
