#include "shell.h"

extern int get_input();

// run commands (recursively)
int spawn(char **command, int in, int out) {

  	pid_t child_pid;
  	int status;
	int result = 0;

  	int i;

  	int input;
  	int output;
  	int append;
  	char *input_filename;
  	char *output_filename;
  	char *append_filename;

  	char **next_command = NULL;
  	if(command[0] != NULL) {
      	printf("Running command: ");
      	for(i = 0; command[i] != NULL; i++) {
        	printf("%s ", command[i]);
      	}
      	printf("\n");

      	// Check for exit and cd
      	if(internal_command(command)) {
          	return 0;
      	}

      	// Check for input/output/append
      	input = input_redir(command, &input_filename);
      	append = check_append(command, &append_filename);
      	output = output_redir(command, &output_filename);

      	// Check for ampersand for backgrounding
		int background = ampersand(command);

      	// Check for pipes and get right side of pipe (next_command)
      	int pipes = check_pipe(command, &next_command);

		char **current = command;
		int fd[2];

		waitpid(WAIT_ANY, &status, WNOHANG);

		if(pipes) {
			child_pid = fork();
			if(child_pid == 0) {
				if(input) {
					freopen(input_filename, "r", stdin);
				}
				if(output) {
					freopen(output_filename, "w+", stdout);
				}
				if(append) {
					freopen(append_filename, "a+", stdout);
				}
				// Child
				while(pipes) {
					pipe(fd);

					result = spawn_process(current, in, fd[1]);

					close(fd[1]);
					in = fd[0];

					// Get next command
					current = next_command;
					next_command = NULL;
					pipes = check_pipe(current, &next_command);
				}

				// Last command
				if(in != 0) {
					dup2(in, 0);
				}

				result = execvp(current[0], current);
			} else {
				// Parent
				child_pid = waitpid(child_pid, &status, 0);
			}
		} else {
			// No pipes
			child_pid = fork();
			if(child_pid == 0) {
				if(input) {
					freopen(input_filename, "r", stdin);
				}
				if(output) {
					freopen(output_filename, "w+", stdout);
				}
				if(append) {
					freopen(append_filename, "a+", stdout);
				}
				result = execvp(command[0], command);
			} else {
				if(background){
					setpgid(child_pid, child_pid);

					tcsetpgrp(STDOUT_FILENO, SHELL_PID);
					tcsetpgrp(STDIN_FILENO, SHELL_PID);

					printf("backgrounded pid: %d\n", child_pid);

					waitpid(child_pid, &status, WNOHANG);
				} else {
					tcsetpgrp(STDIN_FILENO, SHELL_PID);
					tcsetpgrp(STDOUT_FILENO, SHELL_PID);

					printf("simple pid: %d\n", child_pid);

					waitpid(child_pid, &status, 0);
				}
			}
		}
  	}

	if(next_command != NULL) {
		collect_garbage(next_command);
	}
	if(command != NULL) {
		collect_garbage(command);
	}

  	if(result < 0) {
	  	return -1;
  	}

	if(WIFEXITED(status)) {
		if(WEXITSTATUS(status) == 0) {
			return 0;
		} else {
			return -1;
		}
	}

	if(WIFSTOPPED(status)) {
		printf("wifstopped\n");
	}

	return -1;
}

int internal_command(char **command) {
    if(strcmp(command[0], "exit") == 0) {
        exit(0);
    } else if(strcmp(command[0], "cd") == 0) {
        if(command[1] == NULL) {
            printf("cd error: no directory provided\n");
        } else {
            if(chdir(command[1]) != 0) {
                printf("cd error: directory doesn't exist\n");
            }
            return 1;
        }
    }
    return 0;
}

int ampersand(char **command) {
	int i;
	for(i = 0; command[i] != NULL; i++) {
		if(strcmp(command[i], "&") == 0) {
			// Free &
			free(command[i]);

			// Adjust the rest of the command?
			// will there ever be anything after the &?
			command[i] = NULL;

			return 1;
		}
	}
    return 0;
}

int check_pipe(char **command, char ***next_command) {
    int i;
    for(i = 0; command[i] != NULL; i++) {
        if(strcmp(command[i], "|") == 0 && command[i + 1] != NULL) {
        	*next_command = &command[i + 1];
            command[i] = NULL;
            return 1;
        }
    }
    return 0;
}

int input_redir(char **command, char **input_filename) {
    int i;
    int j;

    for(i = 0; command[i] != NULL; i++) {
        // Look for the <
        if(command[i][0] == '<') {
            free(command[i]);

            // Read the filename
            if(command[i + 1] != NULL) {
                *input_filename = command[i + 1];
            } else {
                return -1;
            }

            // Adjust the rest of the arguments in the array
            for(j = i; command[j - 1] != NULL; j++) {
                command[j] = command[j + 2];
            }

            return 1;
        }
    }
    return 0;
}

int output_redir(char **command, char **output_filename) {
    int i;
    int j;

    for(i = 0; command[i] != NULL; i++) {
        // Look for the >
        if(command[i][0] == '>') {
            free(command[i]);

            // Get the filename
            if(command[i + 1] != NULL) {
                *output_filename = command[i + 1];
            } else {
                return -1;
            }

            // Adjust the rest of the arguments in the array
            for(j = i; command[j - 1] != NULL; j++) {
                command[j] = command[j + 2];
            }

            return 1;
        }
    }
    return 0;
}

int check_append(char **command, char **append_filename) {
    int i;
    int j;

    for(i = 0; command[i] != NULL; i++) {
        if(command[i][0] == '>' && command[i][1] == '>') {
            free(command[i]);

            if(command[i + 1] != NULL) {
                *append_filename = command[i + 1];
            } else {
                return -1;
            }

            for(j = i; command[j - 1] != NULL; j++) {
                command[j] = command[j + 2];
            }

            return 1;
        }
    }
    return 0;
}

int spawn_process(char **command, int in, int out) {
	pid_t pid;
	int status;
	int result = 0;

	pid = fork();
	if(pid == 0) {
		// Child
		if(in != 0) {
			dup2(in, 0);
			close(in);
		}

		if(out != 1) {
			dup2(out, 1);
			close(out);
		}

		result = execvp(command[0], command);
	} else {
		// Parent
		pid = waitpid(pid, &status, 0);
	}

	if(result < 0) {
		return -1;
	}

	if(WIFEXITED(status)) {
		if(WEXITSTATUS(status) == 0) {
			return 0;
		} else {
			return -1;
		}
	}

	return -1;
}

void sigchld_handler(int sig) {
	int status;
	while(1) {
		pid_t pid = waitpid(WAIT_ANY, &status, WUNTRACED);
		printf("does this even work: %d\n", pid);
		if(pid < 0) {
			break;
		}
	}
}

int check_and(char **command, char ***next_command) {
    int i;
    for(i = 0; command[i] != NULL; i++) {
        if(strcmp(command[i], "&&") == 0 && command[i + 1] != NULL) {
        	*next_command = &command[i + 1];
            command[i] = NULL;
            return 1;
        }
    }
    return 0;
}

int check_or(char **command, char ***next_command) {
    int i;
    for(i = 0; command[i] != NULL; i++) {
        if(strcmp(command[i], "||") == 0 && command[i + 1] != NULL) {
        	*next_command = &command[i + 1];
            command[i] = NULL;
            return 1;
        }
    }
    return 0;
}

int check_semi(char **command, char ***next_command) {
    int i;
    for(i = 0; command[i] != NULL; i++) {
        if(strcmp(command[i], ";") == 0 && command[i + 1] != NULL) {
        	*next_command = &command[i + 1];
            command[i] = NULL;
            return 1;
        }
    }
    return 0;
}

int do_command(char **command) {
	if(command == NULL) {
		return 0;
	}

	char **next;
	int result = 0;

	if(check_and(command, &next)) {
		result = spawn(command, 0, 1);
		if(result < 0) {
			return result;
		} else {
			return do_command(next);
		}
	} else if(check_or(command, &next)) {
		result = spawn(command, 0, 1);
		if(result < 0) {
			return do_command(next);
		} else {
			return result;
		}
	} else if(check_semi(command, &next)) {
		spawn(command, 0, 1);
		return do_command(next);
	} else {
		return spawn(command, 0, 1);
	}
}

int set_command(char **command) {
	c = command;
	return 0;
}

char **get_command() {
	char **command = c;
	if(command != NULL) {
		c = NULL;
		return command;
	}
	return command;
}

int collect_garbage(char **command) {
	int i;

	for(i = 0; command[i] != NULL; i++) {
		printf("%s\n", command[i]);
		free(command[i]);
	}
	return 0;
}

int main(int argc, char* argv[]) {
    int status;
	int result;
	char **command = NULL;

	//signal(SIGCHLD, sigchld_handler);

    printf("Shell starting with process id: %d\n", SHELL_PID);
	setpgid(SHELL_PID, SHELL_PID);
	tcsetpgrp(STDOUT_FILENO, SHELL_PID);
	tcsetpgrp(STDIN_FILENO, SHELL_PID);

    while(1) {
    	printf("->");
      	status = get_input();
		command = get_command();
		result = 0;
		if(command != NULL) {
			result = do_command(command);
			if(result == -1) {
				printf("command failed: %d\n", result);
			}
		}

    }

    return status;
}
