#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_CMD_LENGTH 1024
#define MAX_ARGS 64

// Function declarations
void parse_command(char *cmd, char **args, char *delimiters);
void execute_command(char **args);
int execute_internal_command(char **args);

int main(int argc, char *argv[]) {
    char cmd[MAX_CMD_LENGTH];
    char *args[MAX_ARGS];

    // Interactive mode
    if (argc == 1) {
        while (1) {
            // Display shell prompt
            printf("$ ");
            fflush(stdout);

            // Read command from stdin
            if (fgets(cmd, MAX_CMD_LENGTH, stdin) == NULL) {
                break; // Exit if end of file
            }

            // Remove newline character
            cmd[strcspn(cmd, "\n")] = '\0';

            // Parse and execute commands
            char *token = strtok(cmd, ";");
            while (token != NULL) {
                parse_command(token, args, " \t");
                if (args[0] != NULL) {
                    execute_command(args);
                }
                token = strtok(NULL, ";");
            }
        }
    }
    // Batch mode
    else if (argc == 2) {
        FILE *batch_file = fopen(argv[1], "r");
        if (batch_file == NULL) {
            perror("Error opening file");
            return EXIT_FAILURE;
        }

        // Read commands from file and execute
        while (fgets(cmd, MAX_CMD_LENGTH, batch_file)) {
            // Remove newline character
            cmd[strcspn(cmd, "\n")] = '\0';

            // Parse and execute commands
            char *token = strtok(cmd, ";");
            while (token != NULL) {
                parse_command(token, args, " \t");
                if (args[0] != NULL) {
                    execute_command(args);
                }
                token = strtok(NULL, ";");
            }
        }

        fclose(batch_file);
    }
    else {
        fprintf(stderr, "Usage: %s [batch_file]\n", argv[0]);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

// Parse command into arguments
void parse_command(char *cmd, char **args, char *delimiters) {
    char *token;
    int i = 0;

    // Tokenize command using delimiters
    token = strtok(cmd, delimiters);
    while (token != NULL) {
        args[i++] = token;
        token = strtok(NULL, delimiters);
    }
    args[i] = NULL; // Set last argument to NULL
}

// Execute command
void execute_command(char **args) {
    int internal = execute_internal_command(args);
    if (!internal) {
        pid_t pid;
        int status;

        pid = fork();
        if (pid < 0) {
            perror("Fork failed");
            exit(EXIT_FAILURE);
        }
        else if (pid == 0) {
            // Child process
            if (execvp(args[0], args) == -1) {
                perror("Execution failed");
                exit(EXIT_FAILURE);
            }
        }
        else {
            // Parent process
            waitpid(pid, &status, 0);
        }
    }
}

// Execute internal command
int execute_internal_command(char **args) {
    if (args[0] == NULL) {
        return 0; // Empty command
    }

    if (strcmp(args[0], "cd") == 0) {
        if (args[1] == NULL) {
            fprintf(stderr, "cd: missing argument\n");
        }
        else {
            if (chdir(args[1]) != 0) {
                perror("cd failed");
            }
        }
        return 1;
    }
    else if (strcmp(args[0], "pwd") == 0) {
        char cwd[1024];
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            printf("%s\n", cwd);
        }
        else {
            perror("getcwd() error");
        }
        return 1;
    }
    else if (strcmp(args[0], "clear") == 0) {
        printf("\033[H\033[J"); // ANSI escape sequence to clear screen
        return 1;
    }
    else if (strcmp(args[0], "exit") == 0) {
        exit(EXIT_SUCCESS);
    }

    return 0; // Not an internal command
}

