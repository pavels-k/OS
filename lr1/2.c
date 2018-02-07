#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>


void child(void)
{
	char* buff = "child proccess ";
	for(int i = 0; i < 10; i++)
	{
		write(1, buff, strlen(buff)); //    1 ?????
		printf("%d\n", i);
		if(i == 6) _exit(0);
	}
}


void parent(void)
{
	char* buff = "parent proccess.";
	for(int i = 10; i < 20; i++)
	{
		write(1, buff, strlen(buff));
		printf("val : %d\n", i);
		if(i == 17) _exit(1);
	}
}


int main(int argc, char const *argv[])
{
	pid_t childPID;
	childPID = fork();
	printf("This is PID:%d\n", childPID);
	if(childPID == 0) child();
	else parent();
	return 0;
}