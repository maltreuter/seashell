CC=gcc
FLAGS=-Wall

all: parser lex shell

parser: parser.y
	yacc -d parser.y

lex: shell_lexer.l
	flex shell_lexer.l

shell: lex.yy.c y.tab.c
	$(CC) lex.yy.c y.tab.c -o shell $(FLAGS)

clean:
	rm lex.yy.c y.tab.* shell
