// Matthew Robinson mbr253

// Problem 1

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <signal.h>

#define BUF_SIZE 1024

static int timedOut;

int myPrint(const char *str) // Problem 2
{
  if (write(1, str, strlen(str)) != strlen(str)) return 0;
  return 1;
}

int myPrintInt(const int val) // Problem 3
{
  char str[BUF_SIZE];
  sprintf(str, "%d", val);
  if (write(1, str, strlen(str)) != strlen(str)) return 0;
  return 1;
}

void signalHandler(int sig) // Problem 4
{
  if (sig == SIGINT)
  {
    char *quest = "\nWould you like to exit? ";
    char ans[BUF_SIZE];
    int numRead;
    while (1)
    {
      if (write(1, quest, strlen(quest)) != strlen(quest))
      {
        perror("write");
        exit(EXIT_FAILURE);
      }
      if ((numRead = read(0, ans, BUF_SIZE)) < 0)
      {
        perror("read");
        exit(EXIT_FAILURE);
      }
      if (strncmp(ans, "Y", 1) == 0 || strncmp(ans, "y", 1) == 0)
      {
        exit(EXIT_SUCCESS);
      }
      if (strncmp(ans, "N", 1) == 0 || strncmp(ans, "n", 1) == 0) return;
    }
  }
  if (sig == SIGALRM)
  {
    timedOut = 1;
    return;
  }
}

int readLine(int fd, char *line) // Problem 5
{
  char let;
  int numRead;
  int i = 0;
  while ((numRead = read(fd, &let, 1)) > 0)
  {
    if (let == '\n' || let == '\r') break;
    line[i] = let;
    i++;
  }
  if (numRead == 0) return 0;
  line[i] = '\0';
  return 1;
}

int readQA(int questFd, int ansFd, char *quest, char *ans)
{
  if (readLine(questFd, quest) == 0) return 0; 
  if (readLine(ansFd, ans) == 0) return 0;
  return 1;
}

int main(int argc, char *argv[])
{

  int numRead = 0;
  int numWrite = 0;
  int question = 1;
  int correct = 0;
  char buf[BUF_SIZE];
  char quest[BUF_SIZE];
  char ans[BUF_SIZE];
  int questFd, ansFd;
  
  // Problem 6
  struct sigaction sa;
  struct itimerval start;
  struct itimerval stop;
  sigemptyset(&sa.sa_mask);
  sa.sa_handler = signalHandler;
  sa.sa_flags = 0;
  start.it_interval.tv_sec = 0;
  start.it_interval.tv_usec = 0;
  start.it_value.tv_sec = 15;
  start.it_value.tv_usec = 0;
  stop.it_interval.tv_sec = 0;
  stop.it_interval.tv_usec = 0;
  stop.it_value.tv_sec = 0;
  stop.it_value.tv_usec = 0;
  
  // Problem 7
  if (sigaction(SIGINT, &sa, NULL) == -1)
  {
    perror("sigaction");
    exit(EXIT_FAILURE);
  }
  if (sigaction(SIGALRM, &sa, NULL) == -1)
  {
    perror("sigaction");
    exit(EXIT_FAILURE);
  }
  
  // Problem 8	
  questFd = open("quest.txt", O_RDONLY);
  ansFd = open("ans.txt", O_RDONLY);
  
  readQA(questFd, ansFd, quest, ans);
  while (1)
  {
    myPrint("#");
    myPrintInt(question);
    myPrint(" ");
    myPrint(quest);
    myPrint("? ");
    timedOut = 0;
    
    if (setitimer(ITIMER_REAL, &start, NULL) == -1) // Problem 9
    {
      perror("settimer");
      exit(EXIT_FAILURE);
    }
      		
    numRead = read(0, &buf, BUF_SIZE); // Problem 10
    
    if (numRead == 0) break;
    if (numRead == -1)
    {
      if (errno == EINTR)
      {
        if (timedOut)
	      {
	        myPrint("\nTime's up, next question\n");
	        if (readQA(questFd, ansFd, quest, ans) == 0) break;
	        question++;
	      }
	      continue;
      }
      perror("read");
      exit(EXIT_FAILURE);
    }
    
    if (setitimer(ITIMER_REAL, &stop, NULL) == -1) // Problem 11
    {
      perror("settimer");
      exit(EXIT_FAILURE);
    }
    
    if (strncmp(buf, ans, strlen(ans)) == 0)
    {
      correct++;
      myPrint("\ncorrect\n");
    } 
    else
    {
      myPrint(ans);
      myPrint("\nwrong\n");
    }
    if (readQA(questFd, ansFd, quest, ans) == 0) break;
    question++;
  }
  myPrint("final score is ");
  myPrintInt(correct);
  myPrint(" out of ");
  myPrintInt(question);
  
  // Problem 12
  close(questFd);
  close(ansFd);
  
  exit(EXIT_SUCCESS);
}
