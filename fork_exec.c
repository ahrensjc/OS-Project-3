#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/sysinfo.h>
#include <string.h>

int GetRamInKB();
int get_memory(int pid);

int main(int argc, char* argv[])
{
  pid_t pid;
  int childPid;
  
  //fork a child process
  printf("ram: %i\n", get_memory(getpid()));
  
  pid = fork();

  if(pid < 0) //fork failed
  {
    fprintf(stderr, "Fork Failed\n");
    return 1;
  }
  else if(pid == 0) //child process
  {  
    printf("I am the child %d mypid is %d\n", pid, getpid());
    printf("ram: %i\n", get_memory(getpid()));
    childPid = getpid();
    
    char* arr[] = {NULL};
    execv("big_function", arr);
  }
  else //parent process
  {
    wait(NULL);
  
    printf("I am the parent %d, mypid is %d\n", pid, getpid());
    printf("ram: %i\n", get_memory(getpid()));
    //printf("The child's memory is: %i\n", get_memory(childPid));
  }
  
  //printf("ram: %i\n", get_memory(getpid()));
  //printf("parent done\n");
  
  return 0;
}

int GetRamInKB()
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
            printf("Free memory: %d (kB)\n", ram);
            fclose(meminfo); 
            return ram;           
        }
    }

    fclose(meminfo);
    return ram;
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