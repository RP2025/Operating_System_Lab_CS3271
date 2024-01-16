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

    // Iterate through the provided executable files and create child processes
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

    // Parent process, wait for each child process to complete
    for (int i = 1; i < argc; i++) {
        int status;
        pid_t pid = waitpid(-1, &status, 0);

        if (pid > 0) {
            if (WIFEXITED(status)) {
                // Child process terminated normally
                printf("Child process %d (PID %d) exited normally with status %d\n", i, pid, WEXITSTATUS(status));
            } else {
                // Child process terminated abnormally
                printf("Child process %d (PID %d) exited abnormally with status %d\n", i, pid, status & 0xFF);
            }
        } else {
            perror("Waitpid error");
            exit(EXIT_FAILURE);
        }
    }

    return 0;
}

