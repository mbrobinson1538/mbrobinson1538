// Matthew Robinson mbr253

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <string.h>

#define BUF_SIZE 1024

int main()
{
	int num, avg, file, fd;
	int total = 0;
	char str[BUF_SIZE];
	ssize_t numRead;
	pid_t pid = fork();
	if (pid == -1)
  	{
  		perror("fork");
  		exit(EXIT_FAILURE);
  	}
  	else if (pid == 0)
  	{
  		if (execlp("./myRand", "./myRand", NULL) == -1)
		{
			perror("execute");
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		wait(&file);
		if (WIFEXITED(file)) file = WEXITSTATUS(file);
		sprintf(str, "data%d.dat", file);
		fd = open(str, O_RDONLY);
		if (fd == -1)
		{
			perror("open for read");
			exit(EXIT_FAILURE);
		}
		while((numRead = read(fd, &num, sizeof(num))) > 0)
		{
			printf("%d\n", num);
			total += num;
		}
		if (numRead == -1)
		{
			perror("read");
			exit(EXIT_FAILURE);
		}
		avg = total / 60;
		printf("%d\n", avg);
		close(fd);
		exit(EXIT_SUCCESS);
	}
}
