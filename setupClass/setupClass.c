#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <dirent.h>
#include <string.h>

int main(int argc, char* argv[])
{
	const char *pathname;
	mode_t mode = ACCESSPERMS;
	if (argc == 1) pathname = "sysProg";
	else if (argc == 2) pathname = argv[1];
	else
	{
		perror("too many arguments");
		exit(EXIT_FAILURE);
	}
	if (mkdir(pathname, mode) == -1)
	{
		perror("directory not made");
		exit(EXIT_FAILURE);
	}
	if (chdir(pathname) == -1)
	{
		perror("directory not changed");
		exit(EXIT_FAILURE);
	}
	if (mkdir("tests", mode) == -1)
	{
		perror("directory not made");
		exit(EXIT_FAILURE);
	}
	if (mkdir("programs", mode) == -1)
	{
		perror("directory not made");
		exit(EXIT_FAILURE);
	}
	if (chdir("tests") == -1)
	{
		perror("directory not changed");
		exit(EXIT_FAILURE);
	}
	for (int i = 1; i < 4; i++)
	{
		char j[32];
		sprintf(j, "%d", i);
		char filename[64] = "test";
		strcat(filename, j);
		if (mkdir(filename, mode) == -1)
		{
			perror("directory not made");
			exit(EXIT_FAILURE);
		}
	}
	if (chdir("..") == -1)
	{
		perror("directory not changed");
		exit(EXIT_FAILURE);
	}
	if (chdir("programs") == -1)
	{
		perror("directory not changed");
		exit(EXIT_FAILURE);
	}
	for (int i = 1; i < 9; i++)
	{
		char j[32];
		sprintf(j, "%d", i);
		char filename[64] = "assn";
		strcat(filename, j);
		if (mkdir(filename, mode) == -1)
		{
			perror("directory not made");
			exit(EXIT_FAILURE);
		}
	}
	for (int i = 1; i < 3; i++)
	{
		char j[32];
		sprintf(j, "%d", i);
		char filename[64] = "assn9_";
		strcat(filename, j);
		if (mkdir(filename, mode) == -1)
		{
			perror("directory not made");
			exit(EXIT_FAILURE);
		}
	}
	exit(EXIT_SUCCESS);
}
