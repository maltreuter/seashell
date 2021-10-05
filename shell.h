#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_ARG_COUNT 128
#define SHELL_PID getpid()

int do_command(char **command, int in, int out);
int internal_command(char **command);
int ampersand(char **command);
int check_pipe(char **command, char ***next_command);
int input_redir(char **command, char **input_filename);
int output_redir(char **command, char **output_filename);
int check_append(char **command, char **output_filename);
int spawn_process(char **command, int in, int out);

#endif //SHELL_H
