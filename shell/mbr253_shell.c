// Matthew Robinson mbr253

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <fcntl.h>

extern char** environ;

int x = -1;
int y, in, out, infd, outfd;
char line[257];
char* cmd[257];
char his[10][257];
int openFlags = O_CREAT | O_WRONLY | O_TRUNC;

void signalHandler(int sig)
{
	// handle sigusr1
	if (sig == SIGUSR1) 
	{
		// create audit.log
		int fd = open("audit.log", openFlags, 0644);
		if (fd == -1)
		{
			perror("open");
			exit(EXIT_FAILURE);
		}
		dup2(fd, 1);
		close(fd);
		// store commands in audit.log
		for (int i = y; i >= 0; i--) printf("%s", his[i]);
		if (x > 10) for (int i = 9; i > y; i--) 
		{
			printf("%s", his[i]);
		}
		exit(EXIT_SUCCESS);
	}
}

int main()
{
	printf("pid: %d\n", (int)getpid());
	int i, and;
	char* pch;
	pid_t pid;
	// initialize signal handler
	struct sigaction sa;
	sa.sa_handler = signalHandler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	// mask sigusr1
	if (sigaction(SIGUSR1, &sa, NULL) == -1)
	{
		perror("sigaction");
		exit(EXIT_FAILURE);
	}
	// read in commands
	while (fgets(line, 256, stdin) != NULL)
	{
		// store command in his
		x++;
		y = x % 10;
	  	strcpy(his[y], line);
	  	// clear cmd
	  	i = 0;
	  	pch = cmd[0];
	  	while (pch != NULL)
	  	{
	  		cmd[i] = NULL;
	  		i++;
	  		pch = cmd[i];
	  	}
	  	// add command to cmd
	  	i = and = in = out = 0;
	  	pch = strtok(line, " \t\n");
	  	while (pch != NULL)
	  	{
	  		if (strcmp(pch, "&") == 0) and = 1; // catch &
	 		// catch <
	  		else if (strcmp(pch, "<") == 0)
	  		{
	  			in = 1;
	  			pch = strtok(NULL, " \t\n");
	  			infd = open(pch, O_RDONLY);
				if (infd == -1)
				{
					perror("open");
					exit(EXIT_FAILURE);
				}
	  		}
	  		// catch >
	  		else if (strcmp(pch, ">") == 0)
	  		{
	  			out = 1;
	  			pch = strtok(NULL, " \t\n");
	  			outfd = open(pch, openFlags, 0644);
				if (outfd == -1)
				{
					perror("open");
					exit(EXIT_FAILURE);
				}
	  		}
	  		else 
	  		{
	  			cmd[i] = pch;
	  			i++;
	  		}
		  	pch = strtok(NULL, " \t\n");
	  	}
		// create child
		pid = fork();
	  	if (pid == -1)
	  	{
	  		perror("fork");
	  		exit(EXIT_FAILURE);
	  	}
	  	// child process
	  	else if (pid == 0)
	  	{
	  		// handle <
	  		if (in == 1)
	  		{
	  			dup2(infd, 0);
	  			close(infd);
	  		}
	  		// handle >
	  		if (out == 1)
	  		{
	  			dup2(outfd, 1);
	  			close(outfd);
	  		}
	  		// execute cd
			if (strcmp(cmd[0], "cd") == 0)
			{
				char dir[257];
				char cwd[257];
				strcpy(dir, cmd[1]);
				// change to home directory
				if (dir == NULL || strcmp(dir, "~") == 0)
				{
					if (chdir(getenv("HOME")) == -1)
					{
						perror("change directory");
						exit(EXIT_FAILURE);
					}
				}
				// change to specific directory
				else if (chdir(dir) == -1)
				{
					perror("change directory");
					exit(EXIT_FAILURE);
				}
				// output current directory
				puts(getcwd(cwd, 256));
				exit(EXIT_SUCCESS);
			}
			// execute history
			else if (strcmp(cmd[0], "history") == 0)
			{
				for (int i = y; i >= 0; i--) 
				{
					printf("%s", his[i]);
				}
				if (x > 9) for (int i = 9; i > y; i--)
				{
					printf("%s", his[i]);
				}
				exit(EXIT_SUCCESS);
			}
			// execute env
			else if (strcmp(cmd[0], "env") == 0)
			{
				int i = 0;
				while (environ[i] != NULL)
				{
					printf("%s\n", environ[i]);
					i++;
				}
				exit(EXIT_SUCCESS);
			}
			else execvp(cmd[0], cmd); //execute commands
	  		printf("This command does not exist.\n");
	  		exit(EXIT_FAILURE);
	  	}
	  	else if (and == 0) wait(NULL); // parent process
  	}
	exit(EXIT_SUCCESS);
}
