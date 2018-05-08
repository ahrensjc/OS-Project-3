/*
  Written By: James Ahrens & Isaac Hendrickson
  May 4, 2018
  Desc: Used to test memory allocation for linuxVM's. Sends a message if memory allocation is above threshold and halts
  processes until memory is below the threshold.
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/sysinfo.h>
#include <string.h>

float get_used_mem(); // Gets used memory in decimal form (0 to 1)
void kill_a_process(); // Selects and kills an appropriate process

// MEMORY THRESHOLD
float THRESHOLD = 0.85;

int main(int argc, char* argv[])
{
  int aboveThreshold = 0; //for making sure you only print once when you go over
  
  while(1)
  {
    float allocatedMem = get_used_mem();
  
    // If memory usage is over the threshold, alert the console and kill a process
    if(allocatedMem > THRESHOLD)
    {
        aboveThreshold = 1;
        printf("YOU'RE USING TOO MUCH MEMORY\n");
        printf("Used memory: %.1f%%\n", allocatedMem*100);
        kill_a_process();
    }
  }
  
  // Wait for 100ms
  usleep(100000);
  return 0;
}

float get_used_mem()
{
  // Open /proc/meminfo
  FILE *meminfo = fopen("/proc/meminfo", "r");
  if(meminfo == NULL)
  {
    printf("Failed to open meminfo!\n");
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

  // Generate command to get the current user's processes
  // Get them in the format "PID RSS VSZ %MEM"
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
  read = getline(&lineBuf, &len, fp); // Trim first line (it only contains column headers)
  while ((read = getline(&lineBuf, &len, fp)) != -1) { // Scan the rest of the output line by line
    sscanf(lineBuf, "%d %d %d %f", &pid, &rss, &vsz, &pmem); // Read each line according to the format spec
    
    // If one matches our criteria, kill it
    // It must not be this process, and it must be using a significant amount of memory
    if((getpid()!=pid) && (rss > 5000)){
      printf("KILLED\tPID: %d\tRSS: %d\tVSZ: %d\t%%mem: %f\n", pid, rss, vsz, pmem);
      kill(pid, SIGKILL);
      return;
    }
  }
  return;
}
