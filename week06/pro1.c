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
        printMessage("I am pi!\n");
        sleep(1); // Sleep for some time
    }

    // Cleanup code (not reached in this example)
    semctl(sem_id, 0, IPC_RMID); // Remove semaphore
    return 0;
}

