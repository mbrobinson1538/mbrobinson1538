// Matthew Robinson mbr253

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

int main()
{
	int fd[2], numRead;
	char in;
	pid_t pid;
	pipe(fd);
	pid = fork();
	if (pid == -1)
  	{
  		perror("fork");
  		exit(EXIT_FAILURE);
  	}
  	else if (pid == 0)
  	{
  		int numRead;
  		char out;
  		close(fd[1]);
  		while ((numRead = read(fd[0], &out, 1)) > 0)
  		{
  			if (write(1, &out, sizeof(out)) != sizeof(out))
  			{
  				perror("write");
				exit(EXIT_FAILURE);
  			}
		}
		if (numRead == -1)
		{
			perror("read");
  			exit(EXIT_FAILURE);
		}
		close(fd[0]);
		exit(EXIT_SUCCESS);
  	}
  	else
  	{
  		close(fd[0]);
  		printf("Enter text to have it repeated.\n");
  		while ((numRead = read(0, &in, 1)) > 0)
  		{
  			if (write(fd[1], &in, sizeof(in)) != sizeof(in))
  			{
  				perror("write");
				exit(EXIT_FAILURE);
  			}
		}
		if (numRead == -1)
		{
			perror("read");
  			exit(EXIT_FAILURE);
		}
		close(fd[1]);
		exit(EXIT_SUCCESS);
  	}
}
