// Matthew Robinson mbr253

// Problem 1

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <time.h>

/* Global vairables that represent if a child's handshake
      has arrived. These globals also represent whether a
      guess is less than (sgn1) or greater than (sgn2) the 
      referee's value */
static int sgn1, sgn2;
int openFlags = O_CREAT | O_WRONLY | O_TRUNC;
mode_t filePerm = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;
pid_t pid[2], rent;
struct sigaction sap;

int checkError(int val, const char *msg)
{
  	if (val == -1)
    	{
      		if (errno == EINTR) return val;
      		perror(msg);
      		exit(EXIT_FAILURE);
    	}
  	return val;
}

void rentHandler(int sig) // Problem 2
{
	if (sig == SIGCHLD)
	{
		if (wait(NULL) == -1)
		{
			if (errno == ECHILD)
			{
				printf("No children remaining.\n");
				exit(EXIT_SUCCESS);
			}
		}
	}
	if (sig == SIGINT)
	{
		kill(pid[0], SIGTERM);
		kill(pid[1], SIGTERM);
		exit(EXIT_SUCCESS);
	}
	if (sig == SIGUSR1) sgn1 = 1;
	if (sig == SIGUSR2) sgn2 = 1;
}

void childHandler(int sig) // Problem 3
{
	if (sig == SIGINT) sgn1 = sgn2 = 2;
	if (sig == SIGTERM) exit(EXIT_SUCCESS);
	if (sig == SIGUSR1) sgn1 = 1;
	if (sig == SIGUSR2) sgn2 = 1;
}

int rngRand(int first, int last)
{
  	int rng = (last - first) + 1;
  	double perc = ((double) rand()) / (((double) RAND_MAX)+1);
  	int offst = rng * perc;
  	return first + offst;
}

int child1() // Problem 4
{
	int min, max, ans, fd;
	struct sigaction sac1;
	sac1.sa_handler = childHandler;
	sigemptyset(&sac1.sa_mask);
	sac1.sa_flags = 0;
  	if (sigaction(SIGINT, &sac1, NULL) == -1)
  	{
  		perror("sigaction");
  		exit(EXIT_FAILURE);
  	}
  	if (sigaction(SIGTERM, &sac1, NULL) == -1)
  	{
  		perror("sigaction");
  		exit(EXIT_FAILURE);
  	}
  	if (sigaction(SIGUSR1, &sac1, NULL) == -1)
  	{
  		perror("sigaction");
  		exit(EXIT_FAILURE);
  	}
  	if (sigaction(SIGUSR2, &sac1, NULL) == -1)
  	{
  		perror("sigaction");
  		exit(EXIT_FAILURE);
  	}
  	while (sgn1 == 0) pause();
  	while(1)
  	{
  		min = 0;
  		max = 100;
		kill(rent, SIGUSR1);
  		while(1)
  		{
  			sgn1 = 0;
  			sgn2 = 0;
  			fd = open("guess1.dat", openFlags, filePerm);
  			if (fd == -1)
			{
				perror("open for write");
				exit(EXIT_FAILURE);
			}
			ans = (max + min) / 2;
			if (write(fd, &ans, sizeof(ans)) != sizeof(ans))
			{
				perror("write");
				exit(EXIT_FAILURE);
			}
			close(fd);
			sleep(1);
			kill(rent, SIGUSR1);
			while(sgn1 == 0 && sgn2 == 0) pause();
			if (sgn1 == 1) min = ans;
			else if (sgn2 == 1) max = ans;
			else break;
		}
  	}
  	exit(EXIT_SUCCESS);
}

int child2() // Problem 5
{
	int min, max, ans, fd;
	struct sigaction sac2;
	sac2.sa_handler = childHandler;
	sigemptyset(&sac2.sa_mask);
	sac2.sa_flags = 0;
  	if (sigaction(SIGINT, &sac2, NULL) == -1)
  	{
  		perror("sigaction");
  		exit(EXIT_FAILURE);
  	}
  	if (sigaction(SIGTERM, &sac2, NULL) == -1)
  	{
  		perror("sigaction");
  		exit(EXIT_FAILURE);
  	}
  	if (sigaction(SIGUSR1, &sac2, NULL) == -1)
  	{
  		perror("sigaction");
  		exit(EXIT_FAILURE);
  	}
  	if (sigaction(SIGUSR2, &sac2, NULL) == -1)
  	{
  		perror("sigaction");
  		exit(EXIT_FAILURE);
  	}
  	while (sgn2 == 0) pause();
  	while(1)
  	{
  		min = 0;
  		max = 100;
		kill(rent, SIGUSR2);
  		while(1)
  		{
  			sgn1 = 0;
  			sgn2 = 0;
  			fd = open("guess2.dat", openFlags, filePerm);
  			if (fd == -1)
			{
				perror("open for write");
				exit(EXIT_FAILURE);
			}
			ans = rngRand(min, max);
			if (write(fd, &ans, sizeof(ans)) != sizeof(ans))
			{
				perror("write");
				exit(EXIT_FAILURE);
			}
			close(fd);
			sleep(1);
			kill(rent, SIGUSR2);
			while(sgn1 == 0 && sgn2 == 0) pause();
			if (sgn1 == 1) min = ans;
			else if (sgn2 == 1) max = ans;
			else break;
		}
  	}
  	exit(EXIT_SUCCESS);
}

void rentPause()
{
  	while (1)
    	{
      		if (sgn1 && sgn2) break;
      		//if (sgn1) break;
      		pause();
    	}
  	sgn1 = sgn2 = 0;
}

int parent() // Problem 6
{
	int ans, ans1, ans2, wins1, wins2, fd1, fd2;
	wins1 = wins2 = 0;
	if (sigaction(SIGINT, &sap, NULL) == -1)
  	{
  		perror("sigaction");
  		exit(EXIT_FAILURE);
  	}
  	sleep(5);
  	kill(pid[0], SIGUSR1);
  	kill(pid[1], SIGUSR2);
  	for (int i = 1; i <= 10; i++)
  	{
  		rentPause();
  		printf("Game %d\n", i);
  		printf("Player 1: %d\n", wins1);
  		printf("Player 2: %d\n", wins2);
  		ans = rngRand(1, 100);
  		printf("Target Answer: %d\n", ans);
  		while(1)
  		{
  			int correct = 0;
  			rentPause();
  			fd1 = open("guess1.dat", O_RDONLY);
  			fd2 = open("guess2.dat", O_RDONLY);
  			if (fd1 == -1 || fd2 == -1)
			{
				perror("open for read");
				exit(EXIT_FAILURE);
			}
			if (read(fd1, &ans1, sizeof(ans1)) == -1)
			{
				perror("read");
				exit(EXIT_FAILURE);
			}
			if (read(fd2, &ans2, sizeof(ans2)) == -1)
			{
				perror("read");
				exit(EXIT_FAILURE);
			}
			close(fd1);
			close(fd2);
			printf("Player 1's Answer: %d\n", ans1);
			printf("Player 2's Answer: %d\n", ans2);
			if (ans1 < ans) kill(pid[0], SIGUSR1);
			else if (ans1 > ans) kill(pid[0], SIGUSR2);
			else
			{
				wins1++;
				correct = 1;
			}
			if (ans2 < ans) kill(pid[1], SIGUSR1);
			else if (ans2 > ans) kill(pid[1], SIGUSR2);
			else
			{
				wins2++;
				correct = 1;
			}
			if (correct == 1) break;
  		}
  		kill(pid[0], SIGINT);
  		kill(pid[1], SIGINT);
  	}
  	printf("Final Score\n");
  	printf("Player 1: %d\n", wins1);
  	printf("Player 2: %d\n", wins2);
}

int main (int argc, char *argv[]) // Problem 7
{
	srand(time(NULL));
	sap.sa_handler = rentHandler;
	sigemptyset(&sap.sa_mask);
	sap.sa_flags = 0;
	rent = getpid();
	if (sigaction(SIGCHLD, &sap, NULL) == -1)
  	{
  		perror("sigaction");
  		exit(EXIT_FAILURE);
  	}
  	if (sigaction(SIGUSR1, &sap, NULL) == -1)
  	{
  		perror("sigaction");
  		exit(EXIT_FAILURE);
  	}
  	if (sigaction(SIGUSR2, &sap, NULL) == -1)
  	{
  		perror("sigaction");
  		exit(EXIT_FAILURE);
  	}
  	pid[0] = fork();
  	if (pid[0] == -1)
  	{
  		perror("fork");
  		exit(EXIT_FAILURE);
  	}
  	if (pid[0] == 0) child1();
  	pid[1] = fork();
  	if (pid[1] == -1)
  	{
  		perror("fork");
  		exit(EXIT_FAILURE);
  	}
  	if (pid[1] == 0) child2();
  	parent();
  	exit(EXIT_SUCCESS);
}
