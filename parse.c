#include "parse.h"

int check_exit(char **command) {
	if(strcmp(command[0], "exit") == 0) {
        return 1;
    }

	return 0;
}

int internal_command(char **command) {
     if(strcmp(command[0], "cd") == 0) {
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
			free(command[i]);
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

int check_and(char **command, char ***next_command) {
    int i;
    for(i = 0; command[i] != NULL; i++) {
        if(strcmp(command[i], "&&") == 0 && command[i + 1] != NULL) {
        	*next_command = &command[i + 1];
			free(command[i]);
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
			free(command[i]);
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
			free(command[i]);
            command[i] = NULL;
            return 1;
        }
    }
    return 0;
}
