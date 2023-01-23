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
	int fd, numRead;
	char in;
	pid_t pid;
	if (mkfifo("fifo", S_IRWXU) == -1)
	{
		if (errno != EEXIST)
		{
			perror("fifo");
			exit(EXIT_FAILURE);
		}
	}
	pid = fork();
	if (pid == -1)
  	{
  		perror("fork");
  		exit(EXIT_FAILURE);
  	}
  	else if (pid == 0)
  	{
  		if (execlp("./fifoOut", "./fifoOut", NULL) == -1)
		{
			perror("execute");
			exit(EXIT_FAILURE);
		}
  	}
  	else
  	{
  		printf("Enter text to have it repeated.\n");
  		fd = open("fifo", O_WRONLY);
  		if (fd == -1)
  		{
  			perror("open for write");
  			exit(EXIT_FAILURE);
  		}
  		while ((numRead = read(0, &in, 1)) > 0)
  		{
  			if (write(fd, &in, sizeof(in)) != sizeof(in))
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
		close(fd);
		exit(EXIT_SUCCESS);
  	}
}
