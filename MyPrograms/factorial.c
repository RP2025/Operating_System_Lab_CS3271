#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

// Function to calculate factorial
unsigned long long factorial(int n) {
    if (n == 0 || n == 1)
        return 1;
    else
        return n * factorial(n - 1);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s n\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);
    if (n <= 0) {
        fprintf(stderr, "Invalid input: n must be a positive integer\n");
        return 1;
    }

    pid_t pid;
    for (int i = 1; i <= n; i++) {
        pid = fork();
        if (pid == -1) {
            perror("fork");
            return 1;
        } else if (pid == 0) {
            // Child process
            printf("Child process %d: Factorial of %d is %llu\n", i, i, factorial(i));
		exit(0);
	    return 0;
        } else {
            // Parent process
            wait(NULL); // Wait for child to finish
        }
    }

    return 0;
}

