#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>

#define SHMSZ sizeof(struct task)

struct task {
    int data[100];
    int size;
    pid_t worker_pid;
    int status;
};

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
            printf("Enter the size of the array (max 100): ");
            scanf("%d", &(solve->size));
            if (solve->size > 100) {
                printf("Size exceeds maximum limit. Setting size to 100.\n");
                solve->size = 100;
            }
            printf("Enter %d elements of the array:\n", solve->size);
            for (int i = 0; i < solve->size; i++) {
                scanf("%d", &(solve->data[i]));
            }
            solve->status = 1;
            printf("Server: Numbers entered for sorting.\n");
        }
        else if (solve->status == 3) {
            // Sorting the array before printing
            bubbleSort(solve->data, solve->size);
            printf("Server: Sorted array by Worker PID %d:\n", solve->worker_pid);
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

