/*
  Written By: James Ahrens & Isaac Hendrickson
  May 4, 2018
  Desc: Used to test memory allocation for linuxVM's. Sends a message if memory allocation is above threshold
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/sysinfo.h>
#include <string.h>

float get_used_mem();

// MEMORY THRESHOLD
float THRESHOLD = 0.85;

int main(int argc, char* argv[])
{
  int aboveThreshold = 0; //for making sure you only print once when you go over
  
  while(1)
  {
    float allocatedMem = get_used_mem();
    
    if((allocatedMem > THRESHOLD) && !aboveThreshold)
    {
        aboveThreshold = 1;
        printf("YOU'RE USING TOO MUCH MEMORY\n");
        printf("Used memory: %.1f%%\n", allocatedMem*100);
    }
    else if (allocatedMem < THRESHOLD)
    {
        aboveThreshold = 0;
    }
    
    // Wait for 100ms
    usleep(100000);
  }
  
  return 0;
}

float get_used_mem()
{
  FILE *meminfo = fopen("/proc/meminfo", "r");
    if(meminfo == NULL)
    {
      exit(1);
    } 

    char line[256];
    int mem, free, used;
    while(fgets(line, sizeof(line), meminfo))
    {
      if (sscanf(line, "MemTotal: %d kB", &mem)==1){
        //printf("Total memory: %d\n", mem);
      }
      if (sscanf(line, "MemFree: %d kB", &free)==1){       
        fclose(meminfo);
        used = mem - free;
        //printf("Used memory: %d\n", used);
        return ((float)used)/((float)mem);
      }
    }
    
    fclose(meminfo);
    return -1;
}
