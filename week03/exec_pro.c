#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    // Check if the correct number of command-line arguments is provided
    if (argc < 2) {
        fprintf(stderr, "Usage: %s executable1 executable2 ... executableN\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Iterate through the executable files and create child processes
    for (int i = 1; i < argc; i++) {
        pid_t pid = fork();

        if (pid == -1) {
            perror("Fork error");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            // Child process
            execlp(argv[i], argv[i], NULL);

            // If execlp fails, print an error message
            perror("Exec error");
            exit(EXIT_FAILURE);
        }
    }

    // Wait for all child processes to complete
    for (int i = 1; i < argc; i++) {
        int status;
        wait(&status);
        printf("Child process %d exited with status %d\n", i, WEXITSTATUS(status));
    }

    return 0;
}

