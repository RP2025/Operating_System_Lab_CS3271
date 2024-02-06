#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>

#define SHMSZ sizeof(struct task)

struct task {
    int task_id;
    int data[100];
    int size;
    pid_t worker_pid;
    int status;
};

// Function to generate random numbers for the array
void generateRandomArray(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        arr[i] = rand() % 1000;
    }
}

// Bubble sort algorithm
void bubbleSort(int arr[], int n) {
    int i, j, temp;
    for (i = 0; i < n-1; i++) {
        for (j = 0; j < n-i-1; j++) {
            if (arr[j] > arr[j+1]) {
                temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
            }
        }
    }
}

int main() {
    const char *shm_name = "task_shared_memory";
    int shmid;
    struct task *solve;
    key_t key = ftok(".", 'T');

    shmid = shmget(key, SHMSZ, IPC_CREAT | 0666);
    if (shmid < 0) {
        perror("shmget");
        exit(1);
    }

    solve = (struct task *)shmat(shmid, NULL, 0);
    if (solve == (struct task *)(-1)) {
        perror("shmat");
        exit(1);
    }

    solve->status = 0;

    while (solve->status != -1) {
        if (solve->status == 0) {
            solve->task_id++;
            solve->size = 100;
            generateRandomArray(solve->data, solve->size);
            solve->status = 1;
            printf("Server: Generated random numbers for task %d.\n", solve->task_id);
        }
        else if (solve->status == 2) {
            printf("Server: Waiting for Worker PID %d to start sorting task %d.\n", solve->worker_pid, solve->task_id);
        }
        else if (solve->status == 3) {
            // Sorting the array before printing
            bubbleSort(solve->data, solve->size);
            printf("Server: Sorted array for task %d by Worker PID %d:\n", solve->task_id, solve->worker_pid);
            for (int i = 0; i < solve->size; i++) {
                printf("%d ", solve->data[i]);
            }
            printf("\n");
            solve->status = 4;
        }
        sleep(1); // To avoid busy waiting
    }

    shmdt((void *)solve);
    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}

