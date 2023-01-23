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
  	char out;
  	fd = open("fifo", O_RDONLY);
  	if (fd == -1)
  	{
  		perror("open for read");
  		exit(EXIT_FAILURE);
  	}
	while ((numRead = read(fd, &out, 1)) > 0)
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
	close(fd);
	exit(EXIT_SUCCESS);
}
