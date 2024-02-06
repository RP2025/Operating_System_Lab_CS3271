#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/sem.h>

#define SEM_KEY 1234

void printMessage(const char *message) {
    printf("%s", message);
    fflush(stdout);
}

int main() {
    key_t sem_key = SEM_KEY;
    int sem_id = semget(sem_key, 1, 0666 | IPC_CREAT);
    if (sem_id == -1) {
        perror("semget");
        exit(EXIT_FAILURE);
    }

    // Infinite loop to print messages
    while (1) {
        // Section 1
        semctl(sem_id, 0, SETVAL, 1); // Initialize semaphore value
        while (semctl(sem_id, 0, GETVAL) == 0) {} // Wait until semaphore is released
        printMessage("I am p2 section 1!\n");
        sleep(1); // Sleep for some time
        semctl(sem_id, 0, SETVAL, 0); // Set semaphore to 0

        // Section 2
        while (semctl(sem_id, 0, GETVAL) == 1) {} // Wait until semaphore is acquired
        printMessage("I am p2 section 2!\n");
        sleep(1); // Sleep for some time
        semctl(sem_id, 0, SETVAL, 1); // Set semaphore to 1
    }

    // Cleanup code (not reached in this example)
    semctl(sem_id, 0, IPC_RMID); // Remove semaphore
    return 0;
}

