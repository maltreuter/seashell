#ifndef PARSE_H
#define PARSE_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int check_exit(char **command);
int internal_command(char **command);
int ampersand(char **command);
int check_pipe(char **command, char ***next_command);
int input_redir(char **command, char **input_filename);
int output_redir(char **command, char **output_filename);
int check_append(char **command, char **output_filename);
int check_and(char **command, char ***next_command);
int check_or(char **command, char ***next_command);
int check_semi(char **command, char ***next_command);

#endif // PARSE_H
