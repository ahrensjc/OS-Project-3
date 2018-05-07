/*
  Written By: James Ahrens & Isaac Hendrickson
  May 4, 2018
  Desc: Used to test memory allocation for linuxVM's.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char** argv)
{  
  pid_t pid = getpid();
  FILE *fp;
  char path[50];
  char pidString[10];
  char username[20];

  getlogin_r(username, 20);
  strcpy(path, "/usr/bin/ps -o pid,rss,vsz -u ");
  strcat(path, username);

  char * lineBuf = NULL; // Stores the whole line
  char * lineData = NULL; // For trimming whitespace
  size_t len = 0;
  ssize_t read;
  int rss, vsz;
  double pmem;


  /* Open the command for reading. */
  fp = popen(path, "r");
  if (fp == NULL) {
    printf("Failed to run command\n" );
    exit(1);
  }
  read = getline(&lineBuf, &len, fp); // Trim first line
  while ((read = getline(&lineBuf, &len, fp)) != -1) {
    printf("Line: %s\n", lineBuf);
    sscanf(lineBuf, "%d %d %d %f", &pid, &rss, &vsz, &pmem);
    printf("PID: %d\tRSS: %d\tVSZ: %d\t%%mem: %f\n", pid, rss, vsz, pmem);
  }

  return 0;
}
