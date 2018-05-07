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

float get_used_mem();
void kill_a_process();

float THRESHOLD = 0.05;

int main(int argc, char* argv[])
{
  //int totalAvailable = get_available();
  int aboveThreshold = 0; //for making sure you only print once when you go over
  
  //
  //while(1)
  while(1)
  {
    float allocatedMem = get_used_mem();
    
    if(allocatedMem > THRESHOLD)
    {
        aboveThreshold = 1;
        printf("YOU'RE USING TOO MUCH MEMORY\n");
        printf("Used memory: %.1f%%\n", allocatedMem*100);
        kill_a_process();
    }
    else if (allocatedMem < THRESHOLD)
    {
        aboveThreshold = 0;
    }
  }

  //printf("%i\n", get_available());
  //printf("%i\n", get_free_mem());
  
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

void kill_a_process()
{
  FILE *fp;
  char path[50];
  char pidString[10];
  char username[20];

  getlogin_r(username, 20);
  strcpy(path, "/usr/bin/ps -o pid,rss,vsz,%mem -u ");
  strcat(path, username);

  char * lineBuf = NULL; // Stores the whole line
  char * lineData = NULL; // For trimming whitespace
  size_t len = 0;
  ssize_t read;
  int pid, rss, vsz;
  float pmem = 1.0;

  /* Open the command for reading. */
  fp = popen(path, "r");
  if (fp == NULL) {
    printf("Failed to run command\n" );
    exit(1);
  }
  read = getline(&lineBuf, &len, fp); // Trim first line
  while ((read = getline(&lineBuf, &len, fp)) != -1) {
    sscanf(lineBuf, "%d %d %d %f", &pid, &rss, &vsz, &pmem);
    
    if((getpid()!=pid) && (rss > 5000)){
      printf("KILLED\tPID: %d\tRSS: %d\tVSZ: %d\t%%mem: %f\n", pid, rss, vsz, pmem);
      kill(pid, SIGKILL);
      return;
    }
  }
  return;
}
