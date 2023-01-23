#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <time.h>

struct Str
{
	char x;
	int y;
};

int main(int argc, char* argv[])
{
	int fd, openFlags, num;
	mode_t filePerm;
	openFlags = O_CREAT | O_WRONLY | O_TRUNC;
	filePerm = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;
	srand(time(NULL));
	if (argc == 1)
	{
		num = 60;
		fd = open("image.dat", openFlags, filePerm);
	}
	else if (argc == 3)
	{
		sscanf(argv[2], "%d", &num);
		fd = open(argv[1], openFlags, filePerm);
	}
	else
	{
		perror("use one or three arguments");
		exit(EXIT_FAILURE);
	}
	if (fd == -1)
	{
		perror("open for write");
		exit(EXIT_FAILURE);
	}
	for (int i = 0; i < num; i++)
	{
		struct Str s;
		s.x = (rand() % 26) + 97;
		s.y = (rand() % 16) + 5;
		if (write(fd, &s, sizeof(struct Str)) != sizeof(struct Str))
		{
			perror("write");
			exit(EXIT_FAILURE);
		}
	}
	if (sizeof(struct Str) == -1)
	{
		perror("read");
		exit(EXIT_FAILURE);
	}
	close(fd);
	exit(EXIT_SUCCESS);
}
