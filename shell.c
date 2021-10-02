#include "shell.h"

extern int get_command();

// run commands (recursively)
int do_command(char **command, int pipe, int input, int ouput) {
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
    printf("input redir\n");
    return 0;
}

int output_redir(char **command, char **output_filename) {
    printf("output redir\n");
    return 0;
}

int append(char **command, char **output_filename) {
    printf("append\n");
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
