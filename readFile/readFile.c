// Matthew Robinson mbr253

#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <dirent.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <sys/times.h>
#include <signal.h>

struct Struct
{
	char x;
	short int y;
};

int main()
{
	int fd;
	ssize_t numRead;
	struct Struct str;
	fd = open("image.dat", O_RDONLY);
	if (fd == -1)
	{
		perror("open for read");
		exit(EXIT_FAILURE);
	}
	while((numRead = read(fd, &str, sizeof(struct Struct))) > 0)
	{
		printf("%c%d ", str.x, str.y);
		for (int i = 0; i < str.y; i++) printf("%c", str.x);
		printf("\n");
		sleep(1);
	}
	if (numRead == -1)
	{
		perror("read");
		exit(EXIT_FAILURE);
	}
	close(fd);
	exit(EXIT_SUCCESS);
}
