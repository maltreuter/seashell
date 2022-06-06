# Seashell
CS 452 - Operating Systems - shell clone

This is a shell clone (e.g. bash, zsh) written in c and lex for Operating Systems.
This code demonstrates an understanding of various system calls: execvp(), fork(), pipe(), and signal().
It can support most commands you would run in your shell with the exception of using parenthesis and quotes due to time constraints.
This assignment is also missing some error handling and unit testing, also due to time constraints.

## Usage
Compile, Run, Clean Up
```
make
./shell
make clean
```

## TO DO
- [x] internal shell commands (exit and cd)
- [x] input, output, append redirection
- [x] pipes
- [x] background processes
- [ ] fg command
- [x] memory leaks
- [ ] parens
- [x] semi colons
- [x] && and ||

yeet
