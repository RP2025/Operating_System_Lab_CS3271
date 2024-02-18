#include<stdio.h>
#include<unistd.h>

int main()
{
	pid_t pid = 0;
	
	printf("parent");

	for(int i = 0 ; i < 5 ; i++)
	{	
		fork();
		printf("hi ");
		
	}
}
