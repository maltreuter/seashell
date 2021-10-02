CC=gcc
FLAGS=-Wall

all: lex shell

lex: lexer.l
	flex lexer.l

shell: lex.yy.c shell.c
	$(CC) lex.yy.c shell.c -o shell $(FLAGS)

clean:
	rm lex.yy.c shell
