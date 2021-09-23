#include "shell.h"

extern int get_command();

int do_command(char **command) {
  pid_t child_pid;
  int status;
  int result;
  int i;

  printf("Running command: ");
  for(i = 0; command[i] != NULL; i++) {
    printf("%s ", command[i]);
  }
  printf("\n");


  child_pid = fork();
  if(child_pid == 0) {
      execvp(command[0], command);
  }

  result = waitpid(child_pid, &status, 0);

  return result;
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
