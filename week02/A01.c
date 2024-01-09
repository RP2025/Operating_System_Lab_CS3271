#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

// Function to calculate factorial
unsigned long long factorial(int num) {
    if (num <= 1)
        return 1;
    else
        return num * factorial(num - 1);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <number>\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);
    pid_t pid;

    if (n <= 0) {
        printf("Please provide a positive integer greater than zero.\n");
        return 1;
    }

    for (int i = 1; i <= n; i++) {
        pid = fork();

        if (pid < 0) {
            printf("Fork failed.\n");
            return 1;
        } else if (pid == 0) { // Child process
            unsigned long long fact = factorial(i);
            printf("Factorial of %d: %llu\n", i, fact);
            exit(0);
        }
    }

    // Parent process waits for all children to complete
    for (int i = 0; i < n; i++) {
        wait(NULL);
    }

    return 0;
}
