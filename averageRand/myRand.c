// Matthew Robinson mbr253

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <string.h>

#define BUF_SIZE 1024

int main()
{
	int randInt, file, fd, openFlags;
	char str[BUF_SIZE];
	mode_t filePerm;
	openFlags = O_CREAT | O_WRONLY | O_TRUNC;
	filePerm = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;
	srand(time(NULL));
	file = rand() % 256;
	sprintf(str, "data%d.dat", file);
	fd = open(str, openFlags, filePerm);
	if (fd == -1)
	{
		perror("open for write");
		exit(EXIT_FAILURE);
	}
	for (int i = 0; i < 60; i++)
	{
		randInt = rand() % 101;
		if (write(fd, &randInt, sizeof(randInt)) != sizeof(randInt))
		{
			perror("write");
			exit(EXIT_FAILURE);
		}
	}
	close(fd);
	exit(file);
}
