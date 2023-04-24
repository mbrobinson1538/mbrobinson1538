// Matthew Robinson mbr253

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>

int main(int argc, char* argv[])
{
	// check if no files
	if (argc == 1)
	{
		perror("no files provided");
		exit(EXIT_FAILURE);
	}
	// check if too many files
	if (argc > 51)
	{
		perror("too many files");
		exit(EXIT_FAILURE);
	}
	// cycle through files
	for (int i = 1; i < argc; i++)
	{
		// open file
		int fd = open(argv[i], O_RDONLY);
		if (fd == -1)
		{
			perror("open");
			exit(EXIT_FAILURE);
		}
		// get info from fstat
		struct stat info;
		if (fstat(fd, &info) != 0)
		{
			perror("fstat");
			exit(EXIT_FAILURE);
		}
		close(fd);
		// output info
		printf("File: %s\n", argv[i]);
		printf("Device ID: %lu\n", info.st_dev);
		printf("Inode Number: %lu\n", info.st_ino);
		printf("Protection: %u\n", info.st_mode);
		printf("Hard Links: %lu\n", info.st_nlink);
		printf("User ID: %u\n", info.st_uid);
		printf("Group ID: %u\n", info.st_gid);
		printf("Special Device ID: %lu\n", info.st_rdev);
		printf("Total Size: %lu\n", info.st_size);
		printf("Block Size: %lu\n", info.st_blksize);
		printf("Blocks Allocated: %lu\n", info.st_blocks);
		printf("Last Access: %lu\n", info.st_atime);
		printf("Last Modification: %lu\n", info.st_mtime);
		printf("Last Status Change: %lu\n", info.st_ctime);
	}
	// get usage from rusage
	struct rusage usage;
	if (getrusage(RUSAGE_SELF, &usage) != 0)
	{
		perror("rusage");
		exit(EXIT_FAILURE);
	}
	// calculate time in microseconds
	int user = usage.ru_utime.tv_sec * 1000000 + usage.ru_utime.tv_usec;
	int sys = usage.ru_stime.tv_sec * 1000000 + usage.ru_stime.tv_usec;
	// output time
	printf("User Time: %d\n", user);
	printf("System Time: %d\n", sys);
	exit(EXIT_SUCCESS);
}
