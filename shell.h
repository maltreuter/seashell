#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_ARG_COUNT 128
#define SHELL_PID getpid()

int do_command(char **command);

#endif //SHELL_H