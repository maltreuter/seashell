#include "shell.h"

extern int get_command();

// run commands (recursively)
int do_command(char **command, int pipe) {
  pid_t child_pid;
  int status;
  int result;

  int i;

  int input;
  int output;
  int append;
  char *input_filename;
  char *output_filename;

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
      append = check_append(command, &output_filename);
      if(!append) {
          output = output_redir(command, &output_filename);
      }

      // Check for ampersand for backgrounding

      // Check for pipes
      next_command = check_pipe(command);
      if(next_command[0] != NULL) {
          printf("\npiped to: ");
          for(i = 0; next_command[i] != NULL; i++) {
              printf("%s ", next_command[i]);
          }
          printf("\n");
      }

      child_pid = fork();
      if(child_pid == 0) {
          if(input) {
              freopen(input_filename, "r", stdin);
          }

          if(output) {
              freopen(output_filename, "w+", stdout);
          }

          if(append) {
              freopen(output_filename, "a+", stdout);
          }

          execvp(command[0], command);
      }

      result = waitpid(child_pid, &status, 0);

      return result;
  }
  return 0;
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
    printf("ampersand\n");
    return 0;
}

char **check_pipe(char **command) {
    int i;
    char **next_command = NULL;
    for(i = 0; command[i] != NULL; i++) {
        if(strcmp(command[i], "|") == 0 && command[i + 1] != NULL) {
            next_command = &command[i + 1];
            command[i] = NULL;
        }
    }
    return next_command;
}

int input_redir(char **command, char **input_filename) {
    int i;
    int j;

    for(i = 0; command[i] != NULL; i++) {
        // Look for the <
        if(command[i][0] == '<') {
            free(command[i]);

            // Read the filename
            if(command[i+1] != NULL) {
                *input_filename = command[i+1];
            } else {
                return -1;
            }

            // Adjust the rest of the arguments in the array
            for(j = i; command[j-1] != NULL; j++) {
                command[j] = command[j+2];
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
            if(command[i+1] != NULL) {
                *output_filename = command[i+1];
            } else {
                return -1;
            }

            // Adjust the rest of the arguments in the array
            for(j = i; command[j-1] != NULL; j++) {
                command[j] = command[j+2];
            }

            return 1;
        }
    }
    return 0;
}

int check_append(char **command, char **output_filename) {
    int i;
    int j;

    for(i = 0; command[i] != NULL; i++) {
        if(command[i][0] == '>' && command[i][1] == '>') {
            free(command[i]);

            if(command[i+1] != NULL) {
                *output_filename = command[i+1];
            } else {
                return -1;
            }

            for(j = i; command[j - 1] != NULL; j++) {
                command[j] = command[j+2];
            }

            return 1;
        }
    }
    return 0;
}

int main(int argc, char* argv[]) {
    int status;

    printf("Shell starting with process id: %d\n", SHELL_PID);

    while(1) {
      printf("->");
      status = get_command();
    }

    return 0;
}
