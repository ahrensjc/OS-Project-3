/*
  Written By: James Ahrens & Isaac Hendrickson
  May 4, 2018
  Desc: Used to test memory allocation for linuxVM's.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int get_memory(int pid)
{  

  FILE *fp;
  char path[50];
  char pidString[10];
  snprintf(pidString, 10, "%d", pid);
  strcpy(path, "/usr/bin/ps -o rss ");
  strcat(path, pidString);

  char * lineBuf = NULL; // Stores the whole line
  char * lineData = NULL; // For trimming whitespace
  size_t len = 0;
  ssize_t read;
  int memSize;


  /* Open the command for reading. */
  fp = popen(path, "r");
  if (fp == NULL) {
    printf("Failed to run command\n" );
    exit(1);
  }

  while ((read = getline(&lineBuf, &len, fp)) != -1) {
    lineBuf[strcspn(lineBuf, "\n\r")] = 0;
    lineData = lineBuf+(2*sizeof(char));
    memSize = atoi(lineData);
    if (memSize != 0)
      break;
  }

  return memSize;
}
