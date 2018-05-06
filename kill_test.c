/*
  Written By: James Ahrens & Isaac Hendrickson
  May 4, 2018
  Desc: Used to test memory allocation for linuxVM's.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

int main(int argc, char **argv)
{  
  pid_t pid = getpid();

  printf("Beginning print\n");
  kill(pid, SIGKILL);
  printf("final print\n");

  return 0;
}
