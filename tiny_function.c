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

int GetRamInKB(void)
{
    FILE *meminfo = fopen("/proc/meminfo", "r");
    if(meminfo == NULL)
    {
      exit(1);
    } 

    char line[256];
    int ram = -1;
    while(fgets(line, sizeof(line), meminfo))
    {
        //printf(line);
        if(sscanf(line, "MemFree: %d kB", &ram) == 1)
        {
            fclose(meminfo);
            return ram;            
        }
    }

    fclose(meminfo);
    return -1;
}

int main( int argc, char* argv[])
{  
  printf("ram: %i\n", get_memory(getpid())); 
  sleep(3);
  
  printf("Tiny function is done\n");
  
  return 0;
}