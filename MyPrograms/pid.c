#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
int main()
{
/* fork a child process */
pid_t pid = fork();
if (pid < 0) { /* error occurred */
fprintf(stderr, "Fork Failed");
printf("parent done");
return 1;
}
if (pid == 0) { /* child process */
	printf("pid is zero");
 //	execlp("/bin/ls","ls",NULL);
}
if (pid > 0) { /* parent process */
/* parent will wait for the child to complete */
wait(NULL);
printf("Child Complete");
}
return 0;
}
