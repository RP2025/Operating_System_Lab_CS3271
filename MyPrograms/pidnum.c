#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
int main()
{
int pp = 0;
	for(int i = 0;i < 5; i++){
/* fork a child process */
pid_t pid = fork();
pp++;

if(pp == 0)
{
        printf("0\n");
}
if(pp == 1)
{
	printf("1\n");

}

if(pp == 2)
{
        printf("2\n");
}if(pp == 3)
{
        printf("3\n");
}if(pp == 4)
{
        printf("4\n");
}if(pp == 5)
{
        printf("termi\n");
}

}
return 0;
}

