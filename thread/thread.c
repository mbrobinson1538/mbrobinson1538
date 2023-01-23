// Matthew Robinson mbr253

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <pthread.h>

#define _POSIX C_SOURCE 199309

void *thread1()
{
	struct timespec tv;
	tv.tv_sec = 1;
	tv.tv_nsec = 0;
	for (int i = 0; i <= 60; i += 2)
	{
		printf("%d\n", i);
		nanosleep(&tv, NULL);
	}
}

void *thread2()
{
	struct timespec tv;
	tv.tv_sec = 1;
	tv.tv_nsec = 0;
	for (int i = 1; i < 60; i += 2)
	{
		printf("%d\n", i);
		nanosleep(&tv, NULL);
	}
}

int main()
{
	pthread_t thr1, thr2;
	pthread_create(&thr1, NULL, thread1, NULL);
	pthread_detach(thr1);
	pthread_create(&thr2, NULL, thread2, NULL);
	pthread_detach(thr2);
	pthread_exit(NULL);
	exit(EXIT_SUCCESS);
}
