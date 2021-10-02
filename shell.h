#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_ARG_COUNT 128
#define SHELL_PID getpid()

int do_command(char **command, int pipe, int input, int output);
int ampersand(char **command);
int check_pipe(char **command);
int input_redir(char **command, char **input_filename);
int output_redir(char **command, char **output_filename);
int append(char **command, char **output_filename);

#endif //SHELL_H
