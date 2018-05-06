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

int main( int argc, char* argv[])
{
  printf("ram: %i\n", GetRamInKB()); 

  int bigArray[1000000];
  int hugeArray[1000][1000];
  
  for(int i = 0; i < 1000000; i++)
  {bigArray[i] = i;}
  
  for(int i = 0; i < 1000; i++)
  {
    for(int j = 0; j < 1000; j++)
    {
      hugeArray[i][j] = i * j;
    }
  }
  
  sleep(3);
  
  printf("ram: %i\n", GetRamInKB()); 
  printf("Big function is done\n");
  
  return 0;
}