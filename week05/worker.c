#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHMSZ sizeof(struct task)

struct task {
    int data[100];
    int size;
    pid_t worker_pid;
    int status;
};

int main() {
    const char *shm_name = "task_shared_memory";
    int shmid;
    struct task *solve;
    key_t key = ftok(".", 'T');

    shmid = shmget(key, SHMSZ, 0666);
    if (shmid < 0) {
        perror("shmget");
        return 1;
    }

    solve = (struct task *)shmat(shmid, NULL, 0);
    if (solve == (struct task *)(-1)) {
        perror("shmat");
        return 1;
    }

    while (solve->status != -1) {
        if (solve->status == 1) {
            solve->status = 2;
            solve->worker_pid = getpid();
            printf("Worker PID %d: Sorting array...\n", getpid());
            // Simulating sorting process
            sleep(2);
            // Once done sorting
            solve->status = 3;
            printf("Worker PID %d: Sorted array.\n", getpid());
        }
        sleep(1); // To avoid busy waiting
    }

    shmdt((void *)solve);

    return 0;
}

