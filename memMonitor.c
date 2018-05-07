/*
  Written By: James Ahrens & Isaac Hendrickson
  May 4, 2018
  Desc: Used to test memory allocation for linuxVM's. Sends a message if memory allocation is above threshold
  NOTE: in proc/meminfo MemAvailable will update with the amount of memory so you can't actually use that you
        have to use MemTotal
  When running use './memMonitor &'
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/sysinfo.h>
#include <string.h>

int get_free_mem();
int get_available();
int get_memory(int pid);

float THRESHOLD = 0.85;

int main(int argc, char* argv[])
{
  //int totalAvailable = get_available();
  int aboveThreshold = 0; //for making sure you only print once when you go over
  
  //
  //while(1)
  for(int i = 0; i < 1; i++)
  {
    int totalAvailable = get_available();
    int totalFree = get_free_mem();
    
    int usedMem = totalAvailable - totalFree;
    
    float allocatedMem = usedMem * 1.0 / totalAvailable;
    
    printf("avail: %i    free: %i    used: %i    percentage: %f%\n", totalAvailable, totalFree, usedMem, allocatedMem);
    //printf("allocated: %f%\n", allocatedMem);
    
    if(allocatedMem > THRESHOLD && !aboveThreshold)
    {
        aboveThreshold = 1;
        printf("YOU'RE USING TOO MUCH MEMORY\n");
    }
    else
    {
        aboveThreshold = 0;
    }
  }

  //printf("%i\n", get_available());
  //printf("%i\n", get_free_mem());
  
  return 0;
}

int get_free_mem()
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
            //printf("Free memory: %d (kB)\n", ram);
            fclose(meminfo); 
            return ram;           
        }
    }

    fclose(meminfo);
    return -1;
}

int get_available()
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
        if(sscanf(line, "MemTotal: %d kB", &ram) == 1)
        {
            //printf("Memory Available: %d (kB)\n", ram);
            fclose(meminfo); 
            return ram;           
        }
    }

    fclose(meminfo);
    return -1;
}

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