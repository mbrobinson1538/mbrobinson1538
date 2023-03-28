// Matthew Robinson mbr253

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>

#define _POSIX C_SOURCE 199309

int total = 0;
pthread_mutex_t lock;

void updateTotal(int lines)
{
	pthread_mutex_lock(&lock); // lock mutex
	total += lines; // add lines to total
	pthread_mutex_unlock(&lock); // unlock mutex
}

void* countLines(void* arg)
{
	int fd, numRead;
	int lines = 0;
	char let;
	// open file
	fd = open(arg, O_RDONLY);
	if (fd == -1)
	{
		perror("open for read");
		exit(EXIT_FAILURE);
	}
	// count lines in file
	while ((numRead = read(fd, &let, 1)) > 0) if (let == '\n') lines++;
	printf("Lines in File: %d\n", lines);
	close(fd);
	updateTotal(lines);
}

int main(int argc, char* argv[])
{
	int args = argc - 1; // get number of files
	// check if no files
	if (args == 0)
	{
		perror("no files provided");
		exit(EXIT_FAILURE);
	}
	pthread_mutex_init(&lock, NULL); // create mutex
	pthread_t thr[args]; // initialize thread array
	// create threads for each file
	for (int i = 0; i < args; i++) 
	{
		pthread_create(&thr[i], NULL, countLines, (void*) argv[i+1]);
		printf("Thread Created\n");
	}
	// join threads
	for (int j = 0; j < args; j++) pthread_join(thr[j], NULL);
	pthread_mutex_destroy(&lock); // destroy mutex
	printf("Total Lines: %d\n", total);
	exit(EXIT_SUCCESS);
}
