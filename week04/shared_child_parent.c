#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>

// Structure for shared memory
struct SharedData {
    int n; // Random number
    long long r; // Factorial of n
};

int main() {
    key_t key = ftok("shmfile",65); // Generate a key for shared memory

    // Create shared memory segment
    int shmid = shmget(key, sizeof(struct SharedData), 0666|IPC_CREAT);
    if (shmid == -1) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    // Attach shared memory to the process
    struct SharedData *shared_data = (struct SharedData*)shmat(shmid, NULL, 0);
    if ((intptr_t)shared_data == -1) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }

    // Fork a child process
    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) { // Child process
        while(1) {
            sleep(1); // Sleep for a regular interval
            printf("Child: Received n = %d, Calculating factorial...\n", shared_data->n);

            // Calculate factorial of n
            long long factorial = 1;
            for (int i = 1; i <= shared_data->n; ++i) {
                factorial *= i;
            }

            shared_data->r = factorial; // Store factorial in shared variable
            printf("Child: Stored factorial (%lld) in r\n", factorial);
        }
    } else { // Parent process
        while(1) {
            sleep(1); // Sleep for a regular interval

            // Generate a random number
            shared_data->n = rand() % 10 + 1; // Random number between 1 and 10
            printf("Parent: Generated random number n = %d\n", shared_data->n);

            printf("Parent: Waiting for child to calculate factorial...\n");
            wait(NULL); // Wait for the child process to finish

            // Print the calculated factorial from shared variable r
            printf("Parent: Received factorial from child: %lld\n", shared_data->r);
        }

        // Detach shared memory
        shmdt(shared_data);

        // Remove shared memory segment
        shmctl(shmid, IPC_RMID, NULL);
    }

    return 0;
}

