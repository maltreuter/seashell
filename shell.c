#include "shell.h"

extern int get_input();
extern int stop_lex();

// Parse and execvp
int spawn_process(char **command, int in, int out) {
  	pid_t child_pid;
  	int status;
	int result = 0;

  	int input;
  	int output;
  	int append;
  	char *input_filename;
  	char *output_filename;
  	char *append_filename;

  	char **next_command = NULL;

	if(command[0] == NULL) {
		return -1;
	}

  	// Check for cd
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

	// Reap em
	waitpid(WAIT_ANY, &status, WNOHANG);

	if(pipes) {
		// Pipe child
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

			while(pipes) {
				pipe(fd);

				result = spawn_pipe_process(current, in, fd[1]);

				close(fd[1]);
				in = fd[0];

				// Get next command
				current = next_command;
				next_command = NULL;
				pipes = check_pipe(current, &next_command);
			}

			// Last command of pipeline
			if(in != 0) {
				dup2(in, 0);
			}

			result = execvp(current[0], current);
		} else {
			// Pipe parent
			child_pid = waitpid(child_pid, &status, 0);
		}
	} else {
		// No pipes child
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

			// Kill child (haha) if execvp fails
			if(result < 0) {
				printf("command failed: %s\n", command[0]);
				collect_garbage(command);
				stop_lex();
				raise(SIGKILL);
			}
		} else {
			// No pipes parent
			if(background) {
				// Create new group id for background process
				setpgid(child_pid, child_pid);

				// Give terminal control back to main
				tcsetpgrp(STDOUT_FILENO, SHELL_PID);
				tcsetpgrp(STDIN_FILENO, SHELL_PID);

				printf("backgrounded pid: %d\n", child_pid);

				waitpid(child_pid, &status, WNOHANG);
			} else {
				// Always give terminal control back to main
				tcsetpgrp(STDIN_FILENO, SHELL_PID);
				tcsetpgrp(STDOUT_FILENO, SHELL_PID);

				waitpid(child_pid, &status, 0);
			}
		}
	}

	// Free filenames and commands
	if(input) {
		free(input_filename);
	}
	if(output) {
		free(output_filename);
	}
	if(append) {
		free(append_filename);
	}
	if(next_command != NULL) {
		collect_garbage(next_command);
	}
	if(command != NULL) {
		collect_garbage(command);
	}

	// Check result of execvp calls
  	if(result < 0) {
	  	return -1;
  	}

	// Check exit status of child process
	if(WIFEXITED(status)) {
		if(WEXITSTATUS(status) == 0) {
			return 0;
		} else {
			return -1;
		}
	}

	return -1;
}

// Execvp helper for pipelines
int spawn_pipe_process(char **command, int in, int out) {
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

	// Check result of execvp calls
	if(result < 0) {
		return -1;
	}

	// Check exit status of child process
	if(WIFEXITED(status)) {
		if(WEXITSTATUS(status) == 0) {
			return 0;
		} else {
			return -1;
		}
	}

	return -1;
}

// Parse for &&, ||, or ;
// Send command to spawn_process for execvp
// Recursive
int do_command(char **command) {
	if(command == NULL) {
		return -1;
	}

	char **next;
	int result = 0;

	if(check_and(command, &next)) {
		result = spawn_process(command, 0, 1);
		if(result < 0) {
			collect_garbage(next);
			return result;
		} else {
			return do_command(next);
		}
	} else if(check_or(command, &next)) {
		result = spawn_process(command, 0, 1);
		if(result < 0) {
			return do_command(next);
		} else {
			collect_garbage(next);
			return result;
		}
	} else if(check_semi(command, &next)) {
		spawn_process(command, 0, 1);
		return do_command(next);
	} else {
		return spawn_process(command, 0, 1);
	}

	return 0;
}

// Pass command from lexer to global char **c
int set_command(char **command) {
	c = command;
	return 0;
}

// Get and clear command from global char **c
char **get_command() {
	char **command = c;
	if(command != NULL) {
		c = NULL;
		return command;
	}
	return command;
}

// Free commands when finished with them
int collect_garbage(char **command) {
	int i;

	for(i = 0; command[i] != NULL; i++) {
		free(command[i]);
	}
	return 0;
}

int main(int argc, char* argv[]) {
	int exit = 0;
	int i;

	char **command = NULL;

    printf("Shell starting with process id: %d\n", SHELL_PID);

	// Set main process group and give group terminal control
	setpgid(SHELL_PID, SHELL_PID);
	tcsetpgrp(STDOUT_FILENO, SHELL_PID);
	tcsetpgrp(STDIN_FILENO, SHELL_PID);

    while(!exit) {
		printf("->");

		// Start lexer
      	get_input();
		command = get_command();

		// Empty command or print
		if(command[0] == NULL) {
			continue;
		} else {
			printf("Running command: ");
		  	for(i = 0; command[i] != NULL; i++) {
		    	printf("%s ", command[i]);
		  	}
		  	printf("\n");
		}

		// Break loop or do command
		if(check_exit(command)) {
			collect_garbage(command);
			exit = 1;
		} else {
			do_command(command);
		}
    }

	// Stop  lexer and free all lexer memory
	stop_lex();

    return 0;
}
