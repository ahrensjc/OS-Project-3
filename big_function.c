/*
  Written By: James Ahrens & Isaac Hendrickson
  May 4, 2018
  Desc: Used to test memory allocation for linuxVM's, generates a large array.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/sysinfo.h>

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

int main( int argc, char* argv[])
{
  printf("ram: %i\n", get_memory(getpid())); 

  int bigArray[1000000];
  //int hugeArray[1000][1000];
  
  for(int i = 0; i < 1000000; i++)
  {bigArray[i] = i;}
  
  //for(int i = 0; i < 1000; i++)
  //{
  //  for(int j = 0; j < 1000; j++)
  //  {
  //    hugeArray[i][j] = i * j;
  //  }
  //}
  
  sleep(3);
  
  printf("ram: %i\n", get_memory(getpid())); 
  printf("Big function is done\n");
  
  return 0;
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
            return ram;
            fclose(meminfo);            
        }
    }

    fclose(meminfo);
    return ram;
}