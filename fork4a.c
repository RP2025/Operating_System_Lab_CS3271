#include <stdio.h>

#include <unistd.h>

int main() {
                 pid_t p;

                 int i;

                  for (i=0; i < 5; i++) {

                        p = fork();
                        if (p == 0) {
                               printf("Child process %d\n!", i+1);

                         } else {
                               printf("Parent process!");
                 }
		  }}
