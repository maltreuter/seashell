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

  char **next_command;

  if(command[0] != NULL) {
      printf("Running command: ");
      for(i = 0; command[i] != NULL; i++) {
        printf("%s ", command[i]);
      }
      printf("\n");

      if(internal_command(command)) {
          printf("internal command\n");
      }

      // Check for input/output/append
      input = input_redir(command, &input_filename);
      output = output_redir(command, &output_filename);
      append = check_append(command, &output_filename);

      // Check for ampersand for backgrounding

      // Check for pipes

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
        // if command[1] == NULL then error for no directory provided
        // else run chdir(command[1])
        // if chdir(command[1]) returns 0, yeet em and skeet em
        // else return error that directory doesnt exist
    }
    return 0;
}

int ampersand(char **command) {
    printf("ampersand\n");
    return 0;
}

int check_pipe(char **command) {
    printf("pipe\n");
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
        if(command[i][0] == '>' && command[i + 1][0] == '>') {
            free(command[i]);
            free(command[i + 1]);

            if(command[i+2] != 0) {
                *output_filename = command[i+2];
            } else {
                return -1;
            }

            for(j = i; command[j - 1] != NULL; j++) {
                command[j] = command[j+3];
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
