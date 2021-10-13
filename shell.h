#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdint.h>

#include "parse.h"

#define MAX_ARG_COUNT 128
#define SHELL_PID getpid()

char **c;

int spawn_process(char **command, int in, int out);
int spawn_pipe_process(char **command, int in, int out);
int do_command(char **command);
int set_command(char **command);
char **get_command();
int collect_garbage(char **command);

#endif //SHELL_H
