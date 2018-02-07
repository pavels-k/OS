#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
/* Not technically required, but needed on some UNIX distributions */
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
int main(int argc, char const *argv[])
{
	char name[] = "current file";
	char data[] = "dannie";
	int fd;
	if((fd = open(name, O_CREAT | O_RDONLY, S_IRUSR | S_IWUSR)) < 0)
	{
		perror(name);
		exit(-1);
	}
	write(fd, data, strlen(data));
	close(fd);
	// remove("current file");
	return 0;
}