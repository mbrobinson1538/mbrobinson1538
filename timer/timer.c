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

void signalHandler(int sig)
{
	if (sig == SIGALRM) return;
}

int main()
{
	int fd;
	ssize_t numRead;
	struct Struct str;
	struct sigaction sa;
	struct itimerval it;
	sigemptyset(&sa.sa_mask);
  	sa.sa_handler = signalHandler;
  	sa.sa_flags = 0;
  	it.it_interval.tv_sec = 0;
  	it.it_interval.tv_usec = 0;
  	it.it_value.tv_sec = 1;
  	it.it_value.tv_usec = 0;
  	if (sigaction(SIGALRM, &sa, NULL) == -1)
  	{
    		perror("sigaction");
    		exit(EXIT_FAILURE);
  	}
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
		if (setitimer(ITIMER_REAL, &it, NULL) == -1)
    		{
      			perror("settimer");
      			exit(EXIT_FAILURE);
    		}
    	pause();
	}
	if (numRead == -1)
	{
		perror("read");
		exit(EXIT_FAILURE);
	}
	close(fd);
	exit(EXIT_SUCCESS);
}
