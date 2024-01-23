#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_SIZE 10

void computeElement(int n, int m, int p, int row, int col, int a[MAX_SIZE][MAX_SIZE], int b[MAX_SIZE][MAX_SIZE], int c[MAX_SIZE][MAX_SIZE]) {
    int result = 0;

    for (int i = 0; i < m; ++i) {
        result += a[row][i] * b[i][col];
    }

    c[row][col] = result;
}

int main() {
    int n, m, p;

    // Read matrix sizes from the user
    printf("Enter the size of matrix A (n X m): ");
    scanf("%d %d", &n, &m);

    printf("Enter the size of matrix B (m X p): ");
    scanf("%d %d", &m, &p);

    // Check if matrix multiplication is possible
    if (m <= 0 || n <= 0 || p <= 0) {
        printf("Invalid matrix sizes for multiplication.\n");
        return EXIT_FAILURE;
    }

    // Initialize matrices A, B, and C
    int a[MAX_SIZE][MAX_SIZE], b[MAX_SIZE][MAX_SIZE], c[MAX_SIZE][MAX_SIZE];

    // Read matrix A from the user
    printf("Enter matrix A (%d X %d):\n", n, m);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            scanf("%d", &a[i][j]);
        }
    }

    // Read matrix B from the user
    printf("Enter matrix B (%d X %d):\n", m, p);
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < p; ++j) {
            scanf("%d", &b[i][j]);
        }
    }

    // Create n*p child processes
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < p; ++j) {
            pid_t pid = fork();

            if (pid < 0) {
                perror("fork");
                exit(EXIT_FAILURE);
            } else if (pid == 0) {
                // Child process
                computeElement(n, m, p, i, j, a, b, c);
                exit(EXIT_SUCCESS);
            }
        }
    }

    // Wait for all child processes to finish
    for (int i = 0; i < n * p; ++i) {
        int status;
        wait(&status);
    }

    // Print the resulting matrix C
    printf("Resulting matrix C (%d X %d):\n", n, p);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < p; ++j) {
            printf("%d ", c[i][j]);
        }
        printf("\n");
    }

    return EXIT_SUCCESS;
}

