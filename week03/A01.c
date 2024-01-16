#include <stdio.h> /* needed also for perror() */
#include <errno.h> /* needed  for perror() */
#include <unistd.h> /* needed for execve() */

int main(int argc, char *argv[]) {
        int status;
        char *myargv[]={"/bin/ls", NULL};
//       int execve(const char *filename, char *const argv[], char *const envp[]);
        status = execve("/bin/ls", myargv, NULL);
        if (status == -1) {
                perror ("Exec Fails: ");
        }

}
