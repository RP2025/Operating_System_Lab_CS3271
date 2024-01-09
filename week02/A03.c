#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

// Function to calculate the value of a term in the polynomial
double evaluateTerm(double coefficient, int exponent, double x) {
    double result = coefficient;
    for (int i = 1; i < exponent; i++) {
        result *= x;
    }
    return result;
}

int main(int argc, char *argv[]) {
    // Check if the correct number of arguments is provided
    if (argc < 4) {
        fprintf(stderr, "Usage: %s v an an-1 an-2 ... a2 a1 a0\n", argv[0]);
        return 1;
    }

    // Convert command line arguments to double and integer values
    double x = atof(argv[1]);
    int n = argc - 3;  // Degree of the polynomial
    double coefficients[n + 1];  // Array to store coefficients

    for (int i = 0; i <= n; i++) {
        coefficients[i] = atof(argv[argc - 1 - i]);
    }

    // Create n+1 child processes
    for (int i = 0; i <= n; i++) {
        pid_t pid = fork();

        if (pid < 0) {
            perror("Fork failed");
            return 1;
        } else if (pid == 0) { // Child process
            double termValue = evaluateTerm(coefficients[i], n - i, x);
            if (i == n) {
                // Print the value of the entire polynomial for v
                printf("Polynomial value for x=%f: %f\n", x, termValue);
            } else {
                // Print the value of the current term
                printf("Term %d value for x=%f: %f\n", i, x, termValue);
            }
            exit(0);
        }
    }

    // Parent process waits for all children to complete
    for (int i = 0; i <= n; i++) {
        wait(NULL);
    }

    return 0;
}

