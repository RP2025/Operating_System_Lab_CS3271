/* fork2.c */

#include <stdio.h>

#include <unistd.h>

int main() {
                 pid_t p;

                 p = fork();

                 if (p == 0) {
                        printf("Child process!");
                 } else {
                        printf("Parent process!");
                 }
}
