#include "shell.h"

extern int get_command();

// run commands (recursively)
int do_command(char **command, int pipe) {
  pid_t child_pid;
  int status;
  int result;
  int i;
  char **next_command;

  if(command[0] != NULL) {
      printf("Running command: ");
      for(i = 0; command[i] != NULL; i++) {
        printf("%s ", command[i]);
      }
      printf("\n");

      // Check for input/output/append

      // Check for ampersand for backgrounding

      // Check for pipes

      child_pid = fork();
      if(child_pid == 0) {
          execvp(command[0], command);
      }

      result = waitpid(child_pid, &status, 0);

      return result;
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

    for(i = 0; args[i] != NULL; i++) {

        // Look for the <
        if(args[i][0] == '<') {
            free(args[i]);

            // Read the filename
            if(args[i+1] != NULL) {
                *input_filename = args[i+1];
            } else {
                return -1;
            }

            // Adjust the rest of the arguments in the array
            for(j = i; args[j-1] != NULL; j++) {
                args[j] = args[j+2];
            }

            return 1;
        }
    }

    return 0;
}

int output_redir(char **command, char **output_filename) {
    int i;
    int j;

    for(i = 0; args[i] != NULL; i++) {

        // Look for the >
        if(args[i][0] == '>') {
            free(args[i]);

            // Get the filename
            if(args[i+1] != NULL) {
                *output_filename = args[i+1];
            } else {
                return -1;
            }

            // Adjust the rest of the arguments in the array
            for(j = i; args[j-1] != NULL; j++) {
                args[j] = args[j+2];
            }

            return 1;
        }
    }

    return 0;
}

int append(char **command, char **output_filename) {
    int i;
    int j;

    for(i = 0; args[i] != NULL; i++) {
        if(args[i][0] == '>' && args[i + 1][0] == '>') {
            free(args[i]);
            free(args[i + 1]);

            if(args[i+2] != 0) {
                *output_filename = args[i+2];
            } else {
                return -1;
            }

            for(j = i; args[j - 1] != NULL; j++) {
                args[j] = args[j+3];
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
