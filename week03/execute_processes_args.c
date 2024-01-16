#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    // Check if the correct number of command-line arguments is provided
    if (argc < 2) {
        fprintf(stderr, "Usage: %s executable1 arg1 arg2 ... executableN arg1 arg2 ...\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int argIndex = 1;

    // Iterate through the provided executable files and create child processes
    while (argIndex < argc) {
        pid_t pid = fork();

        if (pid == -1) {
            perror("Fork error");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            // Child process
            char *executable = argv[argIndex++];
            char *args[argc - argIndex + 1];

            // Extract arguments for the current executable
            int i;
            for (i = 0; argIndex < argc && *argv[argIndex] != '\0'; i++, argIndex++) {
                args[i] = argv[argIndex];
            }

            args[i] = NULL;

            // Execute the current executable with its arguments
            execvp(executable, args);

            // If execvp fails, print an error message
            perror("Exec error");
            exit(EXIT_FAILURE);
        } else {
            // Parent process, wait for the current child to complete
            int status;
            waitpid(pid, &status, 0);
            printf("Child process %d exited with status %d\n", pid, WEXITSTATUS(status));
        }
    }

    return 0;
}

