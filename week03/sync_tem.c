#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <sys/wait.h> 
 
#define MAX_SIZE 10 
 
int A[MAX_SIZE][MAX_SIZE] = { 
    {1, 2, 3}, 
    {4, 5, 6}, 
    {7, 8, 9} 
}; 
 
int B[MAX_SIZE][MAX_SIZE] = { 
    {9, 8, 7}, 
    {6, 5, 4}, 
    {3, 2, 1} 
}; 
 
void computeElement(int m, int n, int r, int row, int col) { 
    int result = 0; 
 
    for (int i = 0; i < n; ++i) { 
        result += A[row][i] * B[i][col]; 
    } 
 
    exit(result); 
} 
 
int main() { 
    int m = 3; 
    int n = 3; 
    int r = 3; 
 
 
    for (int i = 0; i < m; ++i) { 
        for (int j = 0; j < r; ++j) { 
            pid_t pid = fork(); 
 
            if (pid < 0) { 
                perror("fork"); 
                exit(EXIT_FAILURE); 
            } 
            else if (pid == 0) 
            { 
                    computeElement(m, n, r, i, j); 
            } 
    } 
    } 
 
    for (int i = 0; i < m; ++i) { 
        for (int j = 0; j < r; ++j) { 
            int status; 
            pid_t childPID = wait(&status); 
 
 
            int result = WEXITSTATUS(status); 
 
 
            printf("%d ", result); 
 
 
            if (j == r - 1) { 
                printf("\n"); 
            } 
        } 
    } 
 
    return EXIT_SUCCESS; 
}
