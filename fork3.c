/* fork3.c */

#include <stdio.h>

#include <unistd.h>

int main() {
                 pid_t p;

                 p = fork();

                 if (p == 0) {
                       /* program segment B1 to be executed by the child process,

                          keep you own code here for testing! */
			 printf("this is child process alone \n");
                 } else {
                       /* program segment B2 to be executed by the parent process

                          keep you own code here for testing! */
                	 printf("this is parent process alone\n");
		 }

                /* program segment B3 to be executed by both

                   the parent process (if not B2 has terminated it) and

                  the child process (if not B1 has terminated it) ,

                  keep you own code here for testing! */


		  printf("this is both child and parent process \n");
}
