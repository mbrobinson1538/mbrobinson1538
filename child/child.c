// Matthew Robinson mbr253

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <time.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <signal.h>

int x = 0;
int y = 0;
pid_t pid, ppid;

void exitParent()
{
	kill(0, SIGTERM);
}

void exitChild()
{
	kill(ppid, SIGCHLD);
}

void signalHandlerParent(int sig)
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
		char ans;
		while(1)
		{
			printf("\nWould you like to exit? ");
			scanf("%c%*c", &ans);
			if (ans == 'Y' || ans == 'y') exit(EXIT_SUCCESS);
			if (ans == 'N' || ans == 'n') return;
		}
	}
	if (sig == SIGUSR1)
	{
		
		x++;
		printf("The child has generated %d values under 25\n", x);
	}
	if (sig == SIGUSR2)
	{
		y++;
		printf("The child has generated %d values over 75\n", y);
	}
}

void signalHandlerChild(int sig)
{
	if (sig == SIGALRM) return;
	if (sig == SIGTERM) exit(EXIT_SUCCESS);
}

int main()
{
	int z;
	struct sigaction sap;
	struct sigaction sac;
	struct itimerval it;
	sap.sa_handler = signalHandlerParent;
	sigemptyset(&sap.sa_mask);
	sap.sa_flags = 0;
	sac.sa_handler = signalHandlerChild;
	sigemptyset(&sac.sa_mask);
	sac.sa_flags = 0;
	it.it_interval.tv_sec = 0;
  	it.it_interval.tv_usec = 0;
  	it.it_value.tv_sec = 15;
  	it.it_value.tv_usec = 0;
  	srand(time(NULL));
  	ppid = getpid();
  	if (sigaction(SIGCHLD, &sap, NULL) == -1)
  	{
  		perror("sigaction");
  		exit(EXIT_FAILURE);
  	}
  	if (sigaction(SIGINT, &sap, NULL) == -1)
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
  	pid = fork();
  	if (pid == -1)
  	{
  		perror("fork");
  		exit(EXIT_FAILURE);
  	}
  	else if (pid == 0)
  	{
  		sigaddset(&sac.sa_mask, SIGINT);
		sigprocmask(SIG_BLOCK, &sac.sa_mask, NULL);
  		if (sigaction(SIGALRM, &sac, NULL) == -1)
  		{
  			perror("sigaction");
  			exit(EXIT_FAILURE);
  		}
  		if (sigaction(SIGTERM, &sac, NULL) == -1)
  		{
  			perror("sigaction");
  			exit(EXIT_FAILURE);
  		}
  		while(1)
  		{
  			z = rand() % 101;
  			printf("%d\n", z);
  			if (z < 25) kill(ppid, SIGUSR1);
  			if (z > 75) kill(ppid, SIGUSR2);
  			if (z >= 48 && z <= 51) break;
  			if (setitimer(ITIMER_REAL, &it, NULL) == -1)
    			{
      				perror("settimer");
      				exit(EXIT_FAILURE);
  			}
  			pause();
  		}
  		atexit(exitChild);
  		exit(EXIT_SUCCESS);
  	}
  	else
  	{
  		atexit(exitParent);
  		while(1) pause();
  		exit(EXIT_SUCCESS);
  	}
}
