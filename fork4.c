#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
    pid_t p;
    int i;

    for (i = 0; i < 5; i++) {
        p = fork();
        if (p == 0) {
            printf("Child process %d\n", i+1);
            exit(0);
        } else {
            printf("Parent process!\n");
        }
    }

    return 0;
}


