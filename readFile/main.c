#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <time.h>
#include <string.h>

struct pair
{
  char val;
  short int cnt;
};

int myRand(int low, int high)
{
  int rng = high - low + 1;
  static int init;
  double mult;

  
  if (!init) { srand(time(NULL)); init = 1;}

  mult = rand() / (double) RAND_MAX;

  return low + (rng * mult);
}

int myErr(int val, const char *msg)
{
  if (val == -1)
    {
      perror(msg);
      exit(EXIT_FAILURE);
    }
  return val;
}


int main(int argc, char *argv[])
{

  int i = 0;
  int cnt = 60;
  char fname[1024] = "image.dat";
  int fd;
  int numWrit;
  struct pair *arr;

  if (argc > 2)
    {
      strcpy(fname, argv[1]);
      sscanf(argv[2],"%d",&cnt);
    }
      
      


  
  arr = (struct pair *) calloc(cnt, sizeof (struct pair));
  if (arr == NULL) myErr(-1,"calloc");

  
  
  for (i = 0; i < cnt; i++)
    {
      arr[i].val = myRand(97,122);
      arr[i].cnt = myRand(5,20);
    }


  fd = myErr(open(fname,O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR), "open");

  numWrit = myErr(write(fd, arr, cnt * sizeof(struct pair)), "write");
  
  close(fd);
}
